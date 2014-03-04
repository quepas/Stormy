#pragma once

#include <string>
#include "meteo_element.hpp"

namespace stormy {
  namespace meteo {

struct Data 
{ 
  // used for numeric data
  double number;
  // used for text data
  std::string text;
  // ptr to meteo element structure
  Element* element;
};
// ~~ stormy::meteo::Data
}}
