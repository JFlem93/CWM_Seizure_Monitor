//
//  DyNeuMo Filter Design Tool
//  settings.h
//
//  Created by Robert Toth on 2022. 11. 01.
//
//  Based on: https://blog.nickelp.ro/posts/min-guide-to-cli/
//

#ifndef settings_h
#define settings_h

#include <iostream>
#include <functional>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "filter_chain.h"


struct MySettings {
  bool help {false};
  bool double_prec {false};
  bool skip_memcheck {false};
  bool print_coeffs {true};
  std::optional<double> fs;
  std::optional<std::string> output_format;
  std::optional<std::string> in_file;
  std::optional<std::string> out_file;
  std::vector<Filter*> filter_chain;
};


// Toggle flags
typedef std::function<void(MySettings&)> NoArgHandle;

#define S0(str, c) {str, [](MySettings& s) { c; }}
const std::unordered_map<std::string, NoArgHandle> NoArgs {
  S0("-h", {s.help = true;}),
  S0("--help", {s.help = true;}),

  S0("--double", {s.double_prec = true;}),
  
  S0("--skip_memcheck", {s.skip_memcheck = true;}),
  
  //S0("--print_coeffs", {s.print_coeffs = true;}),
  S0("--json", {s.output_format = "json";}),
  S0("--python", {s.output_format = "python";}),
  
  S0("--abs", {s.filter_chain.push_back(new Abs());}),
  S0("--pass", {s.filter_chain.push_back(new Pass());}),
  S0("--exit", {s.filter_chain.push_back(new Exit());})
};
#undef S0


// Single argument flags
typedef std::function<void(MySettings&, const std::string&)> OneArgHandle;

#define S1(str, c) {str, [](MySettings& s, const std::string& arg) { c; }}
const std::unordered_map<std::string, OneArgHandle> OneArgs {
  S1("--output", {s.out_file = arg;}),
  
  S1("--input", {s.in_file = arg;}),
  
  S1("--fs", {
    s.fs = std::stod(arg);
  }),
  
  S1("--offset", {
    s.filter_chain.push_back(new Offset(std::stod(arg)));
  })
};
#undef S1


// Two argument flags
typedef std::function<void(MySettings&, const std::string&,
                           const std::string&)> TwoArgHandle;

#define S2(str, c) {str, [](MySettings& s, const std::string& arg1, \
   const std::string& arg2) { c; }}
const std::unordered_map<std::string, TwoArgHandle> TwoArgs {
  S2("--lowpass", {
    s.filter_chain.push_back(new Lowpass(std::stoi(arg1), std::stod(arg2)));
  }),

  S2("--highpass", {
    s.filter_chain.push_back(new Highpass(std::stoi(arg1), std::stod(arg2)));
  }),

  S2("--movexp", {
    s.filter_chain.push_back(new Movexp(arg1, std::stod(arg2)));
  })
};
#undef S2


// Three argument flags
typedef std::function<void(MySettings&, const std::string&,
                           const std::string&, const std::string&)> ThreeArgHandle;

#define S3(str, c) {str, [](MySettings& s, const std::string& arg1, \
   const std::string& arg2, const std::string& arg3) { c; }}
const std::unordered_map<std::string, ThreeArgHandle> ThreeArgs {
  S3("--bandpass", {
    s.filter_chain.push_back(new Bandpass(std::stoi(arg1), std::stod(arg2), std::stod(arg3)));
  })
};
#undef S3


MySettings parse_settings(int argc, const char* argv[]);


bool missing_arg(std::string opt);
void print_help();

#endif /* settings_h */
