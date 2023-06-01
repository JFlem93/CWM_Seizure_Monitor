//
//  DyNeuMo Filter Design Tool
//  sos.h
//
//  Created by Robert Toth on 2022. 11. 01.
//

#ifndef sos_h
#define sos_h


#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>
#include <complex>
#include <valarray>

//#include "filter_defs.h"
//using SOS_fixed = filter_param_t;


typedef struct {
  int16_t gain;
  int16_t a[3];
  int16_t b[3];
  uint8_t shift_gain;
  uint8_t shift_a;
  uint8_t shift_b;
  uint8_t shift_za;
  uint8_t shift_return;
} SOS_fixed;


typedef struct {
  double gain;
  double a[3];
  double b[3];
} SOS_double;

// Managing SOS
template <typename T>
void fixed_point(T& x_fp, double x_d, int q);

void quantise_sos(SOS_fixed *sos_f, SOS_double *sos_d);
std::string print_sos(SOS_fixed *sos_f, bool quotes = false);
std::string print_sos(SOS_double *sos_d, bool quotes = false);

// Designing coefficients for SOS
void design_sos_bp(SOS_double *sos, int N, double f1, double f2, double fs);
void design_sos_lp(SOS_double *sos, int N, double fc, double fs);
void design_sos_hp(SOS_double *sos, int N, double fc, double fs);


// Aux functions
void poly(std::valarray<std::complex<double>> &a, std::valarray<std::complex<double>> r);
void conv(std::valarray<std::complex<double>> &fg, std::valarray<std::complex<double>> f, std::valarray<std::complex<double>> g);
void eval_tf(std::complex<double>* H, std::valarray<double> b, std::valarray<double> a, double f, double fs);

#endif /* sos_h */
