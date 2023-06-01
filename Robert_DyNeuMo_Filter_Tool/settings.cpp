//
//  DyNeuMo Filter Design Tool
//  settings.cpp
//
//  Created by Robert Toth on 2022. 11. 01.
//
//  Based on: https://blog.nickelp.ro/posts/min-guide-to-cli/
//

#include "settings.h"
#include <iostream>
#include <string>

MySettings parse_settings(int argc, const char* argv[]) {
  MySettings settings;

  // argv[0] is traditionally the program name, so start at 1
  for (int i = 1; i < argc; i++) {
    std::string opt {argv[i]};

    if (auto j {NoArgs.find(opt)}; j != NoArgs.end()) {
      j->second(settings);
    }
    else if (auto k {OneArgs.find(opt)}; k != OneArgs.end()) {
      if (i+1 < argc) {
        if (missing_arg(argv[i+1])) {
          throw std::runtime_error("Missing param after " + opt);
        }
        else {
          k->second(settings, {argv[i+1]});
          i++;
        }
      }
      else {
        throw std::runtime_error("Missing param after " + opt);
      }
    }
    else if (auto l {TwoArgs.find(opt)}; l != TwoArgs.end()) {
      if (i+2 < argc) {
        if (missing_arg(argv[i+1]) || missing_arg(argv[i+2])) {
          throw std::runtime_error("Missing param after " + opt);
        }
        else {
          l->second(settings, {argv[i+1]}, {argv[i+2]});
          i += 2;
        }
      }
      else {
        throw std::runtime_error("Missing param after " + opt);
      }
    }
    else if (auto m {ThreeArgs.find(opt)}; m != ThreeArgs.end()) {
      if (i+3 < argc) {
        if (missing_arg(argv[i+1]) || missing_arg(argv[i+2]) || missing_arg(argv[i+3])) {
          throw std::runtime_error("Missing param after " + opt);
        }
        else {
          m->second(settings, {argv[i+1]}, {argv[i+2]}, {argv[i+3]});
          i += 3;
        }
      }
      else {
        throw std::runtime_error("Missing param after " + opt);
      }
    }
    else {
      throw std::runtime_error("Unrecognized command-line option " + opt);
    }
  }
  
  return settings;
}


bool missing_arg(std::string opt) {
  if (auto tmp {NoArgs.find(opt)}; tmp != NoArgs.end()) {
    return true;
  }
  else if (auto tmp {OneArgs.find(opt)}; tmp != OneArgs.end()) {
    return true;
  }
  else if (auto tmp {TwoArgs.find(opt)}; tmp != TwoArgs.end()) {
    return true;
  }
  else if (auto tmp {ThreeArgs.find(opt)}; tmp != ThreeArgs.end()) {
    return true;
  }
  else {
    return false;
  }
}


void print_help() {
  std::string str = 
    "Required flags:\n"
    "--fs #1\n"
    "Sets sampling rate\n"
    "#1: sampling rate in Hz (double)\n"
    "\n"
    "\n"
    "Optional flags:\n"
    "--help\n"
    "Prints list of available flags, gives instructions on usage\n"
    "\n"
    "--skip_memcheck\n"
    "Disables check on the memory consumption of the provided filter chain\n"
    "\n"
    "--json"
    "Specifies output format as JSON (default)"
    "\n"
    "--python"
    "Specifies output format as python dictionaries"
    "\n"
    "\n"
    "Positional flags:\n"
    "--offset #1\n"
    "Adds a first order high-pass filter to filter chain\n"
    "#1 -3dB cutoff frequency in Hz (double)\n"
    "\n"
    "--movexp #1 #2\n"
    "Adds an exponential moving average stage to filter chain\n"
    "#1 Design mode, specifies the meaning of parameter #2 as one of\n"
    "   ""w"": Window size in samples (integer)\n"
    "   ""t"": Window size in time units of seconds (double)\n"
    "   ""f"": -3dB cutoff frequency in Hz (double)\n"
    "\n"
    "--lowpass #1 #2\n"
    "Adds an even order Butterwortth low-pass filter to filter chain\n"
    "#1 Order (must be even)\n"
    "#2 -3dB cutoff frequency in Hz (double)\n"
    "\n"
    "--highpass #1 #2\n"
    "Adds an even order Butterwortth high-pass filter to filter chain\n"
    "#1 Order (must be even)\n"
    "#2 -3dB cutoff frequency in Hz (double)\n"
    "\n"
    "--bandpass #1 #2 #3\n"
    "#1 Order (must be even)\n"
    "#2 Lower -3dB cutoff frequency in Hz (double)\n"
    "#3 Higher -3dB cutoff frequency in Hz (double)\n"
    "\n"
    "\n"
    "Usage:\n"
    "--flagname[space][parameter][space][parameter]...\n"
    "without brackets, e.g.\n"
    "--offset 8.5 --bandpass 4 18 22 --abs --movexp f 1.2 --lowpass 2 5 --fs 625\n"
    "\n";
  std::cout << str;
}

