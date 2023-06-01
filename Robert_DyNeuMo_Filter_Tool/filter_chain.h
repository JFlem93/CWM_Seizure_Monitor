//
//  DyNeuMo Filter Design Tool
//  filter_chain.h
//
//  Created by Robert Toth on 2022. 10. 31.
//

#ifndef filter_chain_h
#define filter_chain_h

#include <cmath>
#include <complex>
#include <valarray>
#include <string>
#include <algorithm>

#include "sos.h"
#include "filter_types.h"

class Filter {
public:
  filter_function_t type;
  double fs;
  int order;
  
  virtual ~Filter() {}
  virtual void design() = 0;
  virtual void quantise() = 0;
  virtual void set_fs(double fs_) {fs = fs_;}
  virtual std::string print(int i, bool double_prec, bool quotes = false) {return std::string("{}");}
};

class Movexp : public Filter {
public:
  double config_value;
  int design_mode;
  double f0;
  int window;
  uint8_t w_shift;
  Movexp(std::string mode, double config_value_) {
    type = FilterFunctionMovexp;
    order = 1;
    config_value = config_value_;
    if (mode == "w") {
      design_mode = 0;
    } else if (mode == "t") {
      design_mode = 1;
    } else if (mode == "f") {
      design_mode = 2;
    } else {
      throw std::runtime_error("Unknown movexp option " + mode);
    }
  }
  std::string print(int i, bool double_prec, bool quotes = false) {
    std::string s;
    if (double_prec) {
      quotes ? s.append("{\"movexp_window\": ") : s.append("{movexp_window: ");
      s.append(std::to_string(window));
      s.append("}");
    } else {
      quotes ? s.append("{\"movexp_shift\": ") : s.append("{movexp_shift: ");
      s.append(std::to_string(w_shift));
      s.append("}");
    }
    return s;
  }
  void design() {
    switch (design_mode) {
      case 0:
        window = range_limit(std::round(config_value), std::pow(2, 0), std::pow(2, 15));
        f0 = window_to_freq(window);
        break;
      case 1:
        window = range_limit(time_to_window(config_value), std::pow(2, 0), std::pow(2, 15));
        f0 = window_to_freq(window);
        break;
      case 2:
        f0 = (config_value < 1E-5 ) ? 1E-5 : config_value;
        window = range_limit(freq_to_window(f0), std::pow(2, 0), std::pow(2, 15));
        break;
      default:
        throw std::runtime_error("Unknown movexp design mode"); // Should never get this far
        break;
    }
  }
  void quantise() {
    int shift1 = range_limit(std::floor(window_to_shift(window)), 0, 15);
    int shift2 = range_limit(std::ceil(window_to_shift(window)), 0, 15);

    double f1 = window_to_freq(shift_to_window(shift1));
    double f2 = window_to_freq(shift_to_window(shift2));
    
    w_shift = (std::abs(f2 - f0) <= std::abs(f0 - f1)) ? shift2 : shift1;
  }
private:
  int time_to_window(double time) {
    return std::round(time * fs);
  }
  int freq_to_window(double f) {
    const double pi = std::atan(1.0) * 4.0;
    double w0 = f * 2.0 * pi / fs;
    double cos_w0 = std::cos(w0);
    return std::round(-(std::sqrt((cos_w0 - 1.0) * (cos_w0 - 3.0)) - cos_w0 + 1.0) / (2.0 * (cos_w0 - 1.0)));
  }
  double window_to_freq(int w) {
    const double pi = std::atan(1.0) * 4.0;
    double w0 = std::acos((2.0 * std::pow(w, 2.0) - 2.0 * w - 1.0) / (2.0 * w * (w - 1.0)));
    return w0 * fs / (2.0 * pi);
  }
  double window_to_shift(double w) {
    return std::log2(w);
  }
  int shift_to_window(double shift) {
    return std::round(std::pow(2.0, shift));
  }
  int range_limit(int i, int min, int max) {
    return std::min(std::max(min, i), max);
  }
};

class Offset : public Filter {
public:
  double f0;
  double alpha;
  uint8_t alpha_shift;
  Offset(double f0_) {
    type = FilterFunctionOffset;
    order = 1;
    f0 = (f0_ < 1E-5 ) ? 1E-5 : f0_;
  }
  std::string print(int i, bool double_prec, bool quotes = false) {
    std::string s;
    if (double_prec) {
      if (!quotes) {
        s.append("{offset_alpha: ");
      }
      else {
        s.append("{\"offset_alpha\": ");
      }
      s.append(std::to_string(alpha));
      s.append("}");
    } else {
      if (!quotes) {
        s.append("{offset_shift: ");
      }
      else {
        s.append("{\"offset_shift\": ");
      }
      s.append(std::to_string(alpha_shift));
      s.append("}");
    }

    return s;
  }
  void design() {
    alpha = freq_to_alpha(f0);
  }
  void quantise() {
    int shift1 = range_limit(std::floor(alpha_to_shift(alpha)), 2, 15);
    int shift2 = range_limit(std::ceil(alpha_to_shift(alpha)), 2, 15);

    double f1 = alpha_to_freq(shift_to_alpha(shift1));
    double f2 = alpha_to_freq(shift_to_alpha(shift2));

    alpha_shift = (std::abs(f2 - f0) <= std::abs(f0 - f1)) ? shift2 : shift1;
  }
private:
  double freq_to_alpha(double f) {
    const double pi = std::atan(1.0) * 4.0;
    double w0 = f * 2.0 * pi / fs;
    double cos_w0 = std::cos(w0);
    return (cos_w0 - std::sqrt((cos_w0 - 1.0) * (cos_w0 - 3.0))) / (4.0 * cos_w0 - 3.0);
  }
  double alpha_to_freq(double a) {
    const double pi = std::atan(1.0) * 4.0;
    return std::acos((3.0 * std::pow(a, 2.0) - 1.0) / (2.0 * a * (2.0 * a - 1.0))) * fs / (2.0 * pi);
  }
  double alpha_to_shift(double a) {
    return std::log2(-1.0 / (a - 1.0));
  }
  double shift_to_alpha(int shift) {
    return (std::pow(2.0, shift) - 1.0) / std::pow(2.0, shift);
  }
  int range_limit(int i, int min, int max) {
    return std::min(std::max(min, i), max);
  }
};

class Abs : public Filter {
public:
  Abs() {
    type = FilterFunctionAbs;
    order = 1;
  }
  void design() {};
  void quantise() {};
};

class Pass : public Filter {
public:
  Pass() {
    type = FilterFunctionPass;
    order = 1;
  }
  void design() {};
  void quantise() {};
};

class Exit : public Filter {
public:
  Exit() {
    type = FilterFunctionExit;
    order = 1;
  }
  void design() {};
  void quantise() {};
};


class Bandpass : public Filter {
public:
  double f1;
  double f2;
  Bandpass(int N, double f1_, double f2_) {
    type = FilterFunctionBiquad;
    order = N;
    f1 = (f1_ < f2_) ? f1_ : f2_;
    f2 = (f1_ < f2_) ? f2_ : f1_;
    sos_d = new SOS_double[order/2];
    sos_f = new SOS_fixed[order/2];
  }
  ~Bandpass() {
    delete sos_d;
    delete sos_f;
  }
  void design() {
    design_sos_bp(sos_d, order/2, f1, f2, fs);
  }
  void quantise() {
    for (int i = 0; i < order/2; i++) {
      quantise_sos(&sos_f[i], &sos_d[i]);
    }
  }
  std::string print(int i, bool double_prec, bool quotes = false) {
    if (double_prec) {
      return print_sos(&sos_d[i], quotes);
    } else {
      return print_sos(&sos_f[i], quotes);
    }
  }
private:
  SOS_double *sos_d;
  SOS_fixed *sos_f;
};


class Lowpass : public Filter {
public:
  double fc;
  Lowpass(int N, double fc_) {
    type = FilterFunctionBiquad;
    order = N;
    fc = fc_;
    sos_d = new SOS_double[order/2];
    sos_f = new SOS_fixed[order/2];
  }
  ~Lowpass() {
    delete sos_d;
    delete sos_f;
  }
  void design() {
    design_sos_lp(sos_d, order/2, fc, fs);
  }
  void quantise() {
    for (int i = 0; i < order/2; i++) {
      quantise_sos(&sos_f[i], &sos_d[i]);
    }
  }
  std::string print(int i, bool double_prec, bool quotes = false) {
    if (double_prec) {
      return print_sos(&sos_d[i], quotes);
    } else {
      return print_sos(&sos_f[i], quotes);
    }
  }
private:
  SOS_double *sos_d;
  SOS_fixed *sos_f;
};


class Highpass : public Filter {
public:
  double fc;
  Highpass(int N, double fc_) {
    type = FilterFunctionBiquad;
    order = N;
    fc = fc_;
    sos_d = new SOS_double[order/2];
    sos_f = new SOS_fixed[order/2];
  }
  ~Highpass() {
    delete sos_d;
    delete sos_f;
  }
  void design() {
    design_sos_hp(sos_d, order/2, fc, fs);
  }
  void quantise() {
    for (int i = 0; i < order/2; i++) {
      quantise_sos(&sos_f[i], &sos_d[i]);
    }
  }
  std::string print(int i, bool double_prec, bool quotes = false) {
    if (double_prec) {
      return print_sos(&sos_d[i], quotes);
    } else {
      return print_sos(&sos_f[i], quotes);
    }
  }
private:
  SOS_double *sos_d;
  SOS_fixed *sos_f;
};

#endif /* filter_chain_h */
