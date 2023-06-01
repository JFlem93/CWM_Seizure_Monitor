//
//  DyNeuMo Filter Design Tool
//  dyneumo_params.h
//
//  Created by Robert Toth on 2022. 11. 02.
//

#ifndef dyneumo_params_h
#define dyneumo_params_h

#include <unordered_map>

//#include "filter_types.h"

#define DYNEUMO_FILTER_MEM 80

const std::unordered_map<filter_function_t, int> filter_size_map = {
  {FilterFunctionExit, 0},
  {FilterFunctionAbs, 0},
  {FilterFunctionBiquad, 20},
  {FilterFunctionOffset, 2},
  {FilterFunctionMovexp, 2},
  {FilterFunctionPass, 0}
};

#endif /* dyneumo_params_h */
