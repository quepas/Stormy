#pragma once

#include <string>
#include "meteo_element.hpp"
#include "meteo_station.hpp"

namespace stormy {
  namespace meteo {

struct Data
{ 
  // used for numeric data
  double number;
  // used for text data
  std::string text;
  // meteo element
  Element* element;
  // meteo station
  Station* station;
};
// ~~ stormy::meteo::Data
}}
