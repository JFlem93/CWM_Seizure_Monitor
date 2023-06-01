//
//  DyNeuMo Filter Design Tool
//  main.cpp
//
//  Created by Robert Toth on 2022. 10. 12.
//
//  Requirements: C++17 standard library
//

#include "filter_types.h"
#include "dyneumo_params.h"
#include "filter_chain.h"
#include "settings.h"

#include <iostream>

void cleanup(MySettings &settings);


int main(int argc, const char * argv[]) {
  
  MySettings settings;

  // Read settings and init filter chain object
  try {
    settings = parse_settings(argc, argv);
  }
  catch (std::runtime_error& re) {
    std::cerr << re.what() << std::endl;
    cleanup(settings);
    return EXIT_FAILURE;
  }
  catch (...) {
    std::cerr << "Unknown runtime error encountered." << std::endl;
    cleanup(settings);
    return EXIT_FAILURE;
  }
  
  // Print help on usage and exit
  if (settings.help) {
    print_help();
    cleanup(settings);
    return EXIT_SUCCESS;
  }

  // Make sure a filter chain and essential parameters exist
  if (settings.filter_chain.empty()) {
    std::cerr << "Filter chain undefined" << std::endl;
    return EXIT_FAILURE;
  } else if (!settings.fs.has_value()) {
    std::cerr << "Sample rate --fs not provided" << std::endl;
    cleanup(settings);
    return EXIT_FAILURE;
  }
  
  // If memcheck is not disabled, ensure filter chain fits in DyNeuMo memory
  if (!settings.skip_memcheck) {
    int mem_count = 0;
    for (int i = 0; i < settings.filter_chain.size(); i++) {
      mem_count += filter_size_map.at(settings.filter_chain[i]->type);
    }
    if (mem_count > DYNEUMO_FILTER_MEM) {
      std::cerr << "Filter chain exceeds configuration memory, needs ";
      std::cerr << std::to_string(mem_count) << " bytes, got ";
      std::cerr << std::to_string(DYNEUMO_FILTER_MEM) << std::endl;
      return EXIT_FAILURE;
    }
  }
  
  // If all preliminary checks passed, design the filters
  for (int i = 0; i < settings.filter_chain.size(); i++) {
    settings.filter_chain[i]->set_fs(settings.fs.value());
    settings.filter_chain[i]->design();
    settings.filter_chain[i]->quantise();
  }
  
  if (settings.print_coeffs) {
    if (settings.output_format.value_or("json") == "python") {
      // Prints type of each block in filter chain as Python dictionary
      std::string blocks("{");
      int count = 0;
      for (int i = 0; i < settings.filter_chain.size(); i++) {
        std::string s =  "\"" + filter_function_name.at(settings.filter_chain[i]->type) + "\"";
        if (!s.empty()) {
          // Repeat for multi-block filters
          for (int j = 0; j < std::ceil(settings.filter_chain[i]->order/2.0); j++) {
            if (blocks.length() > 1) {
              blocks.append(", ");
            }
            blocks.append(std::to_string(count));
            blocks.append(": ");
            blocks.append(s);
            count++;
          }
        }
      }
      blocks.append("}\n");
      std::cout << "blocks = " << blocks << std::endl;
      
      // Prints coefficients of each block in filter chain as nested Python dictionary
      std::string coeffs("{");
      count = 0;
      for (int i = 0; i < settings.filter_chain.size(); i++) {
        // Repeat for multi-block filters
        for (int j = 0; j < std::ceil(settings.filter_chain[i]->order/2.0); j++) {
          if (coeffs.length() > 1) {
            coeffs.append(", ");
          }
          coeffs.append(std::to_string(count));
          coeffs.append(": ");
          coeffs.append(settings.filter_chain[i]->print(j, settings.double_prec));
          count++;
        }
      }
      coeffs.append("}\n");
      std::cout << "coeffs = " << coeffs << std::endl;
    }
    else {
      // Prints type of each block in filter chain as Python dictionary
      std::cout << "{" << std::endl;
      std::string blocks("{");
      int count = 0;
      for (int i = 0; i < settings.filter_chain.size(); i++) {
        std::string s =  "\"" + filter_function_name.at(settings.filter_chain[i]->type) + "\"";
        if (!s.empty()) {
          // Repeat for multi-block filters
          for (int j = 0; j < std::ceil(settings.filter_chain[i]->order/2.0); j++) {
            if (blocks.length() > 1) {
              blocks.append(", ");
            }
            blocks.append("\"" + std::to_string(count) + "\"");
            blocks.append(": ");
            blocks.append(s);
            count++;
          }
        }
      }
      blocks.append("},");
      std::cout << "\"blocks\" : " << blocks << std::endl;
      
      // Prints coefficients of each block in filter chain as nested Python dictionary
      std::string coeffs("{");
      count = 0;
      for (int i = 0; i < settings.filter_chain.size(); i++) {
        // Repeat for multi-block filters
        for (int j = 0; j < std::ceil(settings.filter_chain[i]->order/2.0); j++) {
          if (coeffs.length() > 1) {
            coeffs.append(", ");
          }
          coeffs.append("\"" + std::to_string(count) + "\"");
          coeffs.append(": ");
          coeffs.append(settings.filter_chain[i]->print(j, settings.double_prec, true));
          count++;
        }
      }
      coeffs.append("}");
      std::cout << "\"coeffs\" : " << coeffs << std::endl;
    }
    std::cout << "}" << std::endl;
  }
  

  cleanup(settings);
  
  return EXIT_SUCCESS;
}


// Release dynamically allocated memory
void cleanup(MySettings &settings) {
  for (int i = 0; i < settings.filter_chain.size(); i++) {
    delete settings.filter_chain[i];
  }
}
