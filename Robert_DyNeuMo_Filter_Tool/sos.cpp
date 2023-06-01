//
//  DyNeuMo Filter Design Tool
//  sos.cpp
//
//  Created by Robert Toth on 2022. 11. 01.
//

#include "sos.h"

#include <stdio.h>
#include <cmath>

// Converts double precision variable to arbitrary signed fixed point integer formats
template <typename T>
void fixed_point(T& x_fp, double x_d, int q) {
  if (x_d >= std::pow(2, 15 - q)) {
    x_fp = static_cast<T>(std::numeric_limits<T>::max());
  }
  else if (x_d < -std::pow(2, 15 - q)) {
    x_fp = static_cast<T>(std::numeric_limits<T>::min());
  }
  else {
    x_fp = static_cast<T>(std::round(x_d * std::pow(2, q)));
  }
}

void quantise_sos(SOS_fixed *sos_f, SOS_double *sos_d) {
  sos_f->shift_a = 14;  // Empirical constants
  sos_f->shift_b = 14;
  sos_f->shift_za = 0;
  sos_f->shift_return = 15;
  sos_f->shift_gain = 16 - std::floor(std::log2(sos_d->gain) + 2) + 15 - 30; // Don't try to think about it
  
  fixed_point(sos_f->gain, sos_d->gain, sos_f->shift_gain + 30 - 15);
  
  fixed_point(sos_f->a[0], sos_d->a[0], sos_f->shift_a);
  fixed_point(sos_f->a[1], sos_d->a[1], sos_f->shift_a);
  fixed_point(sos_f->a[2], sos_d->a[2], sos_f->shift_a);
  
  fixed_point(sos_f->b[0], sos_d->b[0], sos_f->shift_b);
  fixed_point(sos_f->b[1], sos_d->b[1], sos_f->shift_b);
  fixed_point(sos_f->b[2], sos_d->b[2], sos_f->shift_b);
}

std::string print_sos(SOS_fixed *sos_f, bool quotes) {
  std::string s("{");
  if (!quotes) {
    s.append("gain: "); s.append(std::to_string(sos_f->gain)); s.append(", ");
    s.append("a0: "); s.append(std::to_string(sos_f->a[0])); s.append(", ");
    s.append("a1: "); s.append(std::to_string(sos_f->a[1])); s.append(", ");
    s.append("a2: "); s.append(std::to_string(sos_f->a[2])); s.append(", ");
    s.append("b0: "); s.append(std::to_string(sos_f->b[0])); s.append(", ");
    s.append("b1: "); s.append(std::to_string(sos_f->b[1])); s.append(", ");
    s.append("b2: "); s.append(std::to_string(sos_f->b[2])); s.append(", ");
    s.append("shift_gain: "); s.append(std::to_string(sos_f->shift_gain)); s.append(", ");
    s.append("shift_a: "); s.append(std::to_string(sos_f->shift_a)); s.append(", ");
    s.append("shift_b: "); s.append(std::to_string(sos_f->shift_b)); s.append(", ");
    s.append("shift_za: "); s.append(std::to_string(sos_f->shift_za)); s.append(", ");
    s.append("shift_return: "); s.append(std::to_string(sos_f->shift_return));
  }
  else {
    s.append("\"gain\": "); s.append(std::to_string(sos_f->gain)); s.append(", ");
    s.append("\"a0\": "); s.append(std::to_string(sos_f->a[0])); s.append(", ");
    s.append("\"a1\": "); s.append(std::to_string(sos_f->a[1])); s.append(", ");
    s.append("\"a2\": "); s.append(std::to_string(sos_f->a[2])); s.append(", ");
    s.append("\"b0\": "); s.append(std::to_string(sos_f->b[0])); s.append(", ");
    s.append("\"b1\": "); s.append(std::to_string(sos_f->b[1])); s.append(", ");
    s.append("\"b2\": "); s.append(std::to_string(sos_f->b[2])); s.append(", ");
    s.append("\"shift_gain\": "); s.append(std::to_string(sos_f->shift_gain)); s.append(", ");
    s.append("\"shift_a\": "); s.append(std::to_string(sos_f->shift_a)); s.append(", ");
    s.append("\"shift_b\": "); s.append(std::to_string(sos_f->shift_b)); s.append(", ");
    s.append("\"shift_za\": "); s.append(std::to_string(sos_f->shift_za)); s.append(", ");
    s.append("\"shift_return\": "); s.append(std::to_string(sos_f->shift_return));
  }
  s.append("}");

  return s;
}

std::string print_sos(SOS_double *sos_d, bool quotes) {
  std::string s("{");
  if (!quotes) {
    s.append("gain: "); s.append(std::to_string(sos_d->gain)); s.append(", ");
    s.append("a0: "); s.append(std::to_string(sos_d->a[0])); s.append(", ");
    s.append("a1: "); s.append(std::to_string(sos_d->a[1])); s.append(", ");
    s.append("a2: "); s.append(std::to_string(sos_d->a[2])); s.append(", ");
    s.append("b0: "); s.append(std::to_string(sos_d->b[0])); s.append(", ");
    s.append("b1: "); s.append(std::to_string(sos_d->b[1])); s.append(", ");
    s.append("b2: "); s.append(std::to_string(sos_d->b[2]));
  }
  else {
    s.append("\"gain\": "); s.append(std::to_string(sos_d->gain)); s.append(", ");
    s.append("\"a0\": "); s.append(std::to_string(sos_d->a[0])); s.append(", ");
    s.append("\"a1\": "); s.append(std::to_string(sos_d->a[1])); s.append(", ");
    s.append("\"a2\": "); s.append(std::to_string(sos_d->a[2])); s.append(", ");
    s.append("\"b0\": "); s.append(std::to_string(sos_d->b[0])); s.append(", ");
    s.append("\"b1\": "); s.append(std::to_string(sos_d->b[1])); s.append(", ");
    s.append("\"b2\": "); s.append(std::to_string(sos_d->b[2]));
  }
  s.append("}");

  return s;
}


// Code based on the following resources:
// (1) Neil Robertson (2018) 'Design IIR Bandpass Filters', DSPRelated,
// available at: https://www.dsprelated.com/showarticle/1128.php
// (2) Neil Robertson (2019) 'IIR Bandpass Filters Using Cascaded Biquads',
// DSPRelated, available at: https://www.dsprelated.com/showarticle/1257.php
//
// This implementation fixes an issue from (2) that would have constrained
// odd-N section cascades to BW < 2*F0.
//
void design_sos_bp(SOS_double *sos, int N, double f1, double f2, double fs) {  
  using namespace std::complex_literals;
  const double pi = std::atan(1.0) * 4.0;
    
  // 1. Find the poles of a lowpass analog prototype filter
  std::valarray<std::complex<double>> k(0.0, N);
  for (int j = 1; j <= N; j++) {
    k[j-1] = j;
  }
  std::valarray<std::complex<double>> theta = (2*k - 1) * pi / (2*N);
  std::valarray<std::complex<double>> p_lp = -std::sin(theta) + 1.0i * std::cos(theta);
  
  // 2. Find the frequencies of the analog bandpass filter with pre-warping
  double F1 = fs/pi * std::tan(pi*f1/fs);
  double F2 = fs/pi * std::tan(pi*f2/fs);
  double F0 = std::sqrt(F1 * F2);
  double BW = F2 - F1;
  
  // 3. Transform the analog lowpass poles to analog bandpass poles
  std::valarray<std::complex<double>> alpha = BW/(F0*2.0) * p_lp;
  std::valarray<std::complex<double>> beta = 1.0i * std::sqrt(1.0 - std::pow((BW/(F0*2.0) * p_lp), 2.0));
  
  std::valarray<std::complex<double>> pa1 = 2*pi*F0 * (alpha - beta);
  std::valarray<std::complex<double>> pa2 = 2*pi*F0 * (alpha + beta);
  
  // 4. Transform the poles from the s-plane to the z-plane
  std::valarray<std::complex<double>> p1 = (1 + pa1/(2*fs)) / (1 - pa1/(2*fs));
  std::valarray<std::complex<double>> p2 = (1 + pa2/(2*fs)) / (1 - pa2/(2*fs));
  
  // 5. Add N zeros at z=-1, and N zeros at z=+1, distribute between sections
  std::valarray<std::complex<double>> q = {-1, +1};
  
  // For each section
  for (int j = 0; j < N; j++) {
    // 6. Convert poles and zeros to polynomials with coefficients an and bn
    std::valarray<std::complex<double>> a;
    std::valarray<std::complex<double>> b;
    poly(a, {p1[N-1-j], p2[j]});
    poly(b, q);
    
    sos[j].a[0] = a[0].real();
    sos[j].a[1] = a[1].real();
    sos[j].a[2] = a[2].real();
    sos[j].b[0] = b[0].real();
    sos[j].b[1] = b[1].real();
    sos[j].b[2] = b[2].real();
    
    // 7. Set gain to unity at geometric mean of corner frequencies
    std::complex<double> Hf0;
    std::valarray<double> a_real = {a[0].real(), a[1].real(), a[2].real()};
    std::valarray<double> b_real = {b[0].real(), b[1].real(), b[2].real()};
    eval_tf(&Hf0, b_real, a_real, std::sqrt(f1*f2), fs);
    sos[j].gain = 1/std::abs(Hf0);
  }
}

// Based on:
// Neil Robertson (2018) 'Design IIR Filters Using Cascaded Biquads',
// available at: https://www.dsprelated.com/showarticle/1137.php
//
void design_sos_lp(SOS_double *sos, int N, double fc, double fs) {
  using namespace std::complex_literals;
  const double pi = std::atan(1.0) * 4.0;
    
  // 1. Find the poles of a lowpass analog prototype filter
  std::valarray<std::complex<double>> k(0.0, N);
  for (int j = 1; j <= N; j++) {
    k[j-1] = j;
  }
  std::valarray<std::complex<double>> theta = (2*k - 1) * pi / (2* 2*N);
  std::valarray<std::complex<double>> pa = -std::sin(theta) + 1.0i * std::cos(theta);
  
  // 2. Find the frequencies of the analog lowpass filter with pre-warping
  double FC = fs/pi * std::tan(pi*fc/fs);
  pa = 2*pi*FC * pa;
  
  // 3. Transform the poles from the s-plane to the z-plane
  std::valarray<std::complex<double>> p = (1 + pa/(2*fs)) / (1 - pa/(2*fs));
  
  // For each section
  for (int j = 0; j < N; j++) {
    // 4. Set coefficients
    sos[j].a[0] = 1.0;
    sos[j].a[1] = -2.0 * p[N-1-j].real();
    sos[j].a[2] = std::pow(std::abs(p[j]), 2.0);
    sos[j].b[0] = 1.0;
    sos[j].b[1] = 2.0;
    sos[j].b[2] = 1.0;
    
    // 5. Set gain to unity at DC
    sos[j].gain = (sos[j].a[0] + sos[j].a[1] + sos[j].a[2]) / 4;
  }
}

void design_sos_hp(SOS_double *sos, int N, double fc, double fs) {
  using namespace std::complex_literals;
  const double pi = std::atan(1.0) * 4.0;
    
  // 1. Find the poles of a lowpass analog prototype filter
  std::valarray<std::complex<double>> k(0.0, N);
  for (int j = 1; j <= N; j++) {
    k[j-1] = j;
  }
  std::valarray<std::complex<double>> theta = (2*k - 1) * pi / (2* 2*N);
  std::valarray<std::complex<double>> pa = -std::sin(theta) + 1.0i * std::cos(theta);
  
  // 2. Find the frequencies of the analog lowpass filter with pre-warping
  double FC = fs/pi * std::tan(pi*fc/fs);
  
  // 3. Transform the analog lowpass poles to analog bandpass poles
  pa = 2*pi*FC / pa;
  
  // 4. Transform the poles from the s-plane to the z-plane
  std::valarray<std::complex<double>> p = (1 + pa/(2*fs)) / (1 - pa/(2*fs));
  
  // For each section
  for (int j = 0; j < N; j++) {
    // 5. Set coefficients
    sos[j].a[0] = 1.0;
    sos[j].a[1] = -2.0 * p[N-1-j].real();
    sos[j].a[2] = std::pow(std::abs(p[j]), 2.0);
    sos[j].b[0] = 1.0;
    sos[j].b[1] = -2.0;
    sos[j].b[2] = 1.0;
    
    // 6. Set gain to unity at DC
    sos[j].gain = (sos[j].a[0] - sos[j].a[1] + sos[j].a[2]) / 4;
  }
}

void poly(std::valarray<std::complex<double>> &a, std::valarray<std::complex<double>> r) {
  a.resize(r.size()+1, 1);
  for (int i = 0; i < r.size(); i++) {
    std::valarray<std::complex<double>> root = {1, -r[i]};
    conv(a, a[std::slice(0,i+1,1)], root);
  }
}

void conv(std::valarray<std::complex<double>> &fg, std::valarray<std::complex<double>> f, std::valarray<std::complex<double>> g) {
  size_t M = f.size();
  size_t N = g.size();
  fg.resize(M+N-1, 0);
  for (int m = 0; m < M; m++) {
    for (int n = 0; n < N; n++) {
      fg[m+n] = fg[m+n] + f[m] * g[n];
    }
  }
}

void eval_tf(std::complex<double> *H, std::valarray<double> b, std::valarray<double> a, double f, double fs) {
  using namespace std::complex_literals;
  const double pi = std::atan(1.0) * 4.0;
  // Express test frequency in z domain
  std::complex<double> z = std::exp(2i * pi * f / fs);
  // Numerator
  std::complex<double> num = 0;
  for (int j = 0; j < b.size(); j++) {
    num += b[j] * std::pow(z, -j);
  }
  // Denominator
  std::complex<double> den = 0;
  for (int j = 0; j < a.size(); j++) {
    den += a[j] * std::pow(z, -j);
  }
  *H = (num / den);
}
