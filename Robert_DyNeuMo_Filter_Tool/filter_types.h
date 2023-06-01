//
//  DyNeuMo Filter Design Tool
//  filter_types.h
//
//  Created by Robert Toth on 2022. 11. 29.
//

#ifndef filter_types_h
#define filter_types_h

#include <unordered_map>
#include <string>

typedef enum {
  FilterFunctionExit,
  FilterFunctionAbs,
  FilterFunctionBiquad,
  FilterFunctionOffset,
  FilterFunctionMovexp,
  FilterFunctionPass
} filter_function_t;

const std::unordered_map<filter_function_t, std::string> filter_function_name = {
  {FilterFunctionExit,   "exit"},
  {FilterFunctionAbs,    "abs"},
  {FilterFunctionBiquad, "sos"},
  {FilterFunctionOffset, "offset"},
  {FilterFunctionMovexp, "movexp"},
  {FilterFunctionPass,   "pass"}
};

#endif /* filter_types_h */
