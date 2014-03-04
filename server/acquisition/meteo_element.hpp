#pragma once

#include <string>

namespace stormy {
  namespace meteo {

struct Element
{
  std::string id;
  // equivalent element names in local language
  std::string names;
  // value type (e.q. numeric, percent, text)
  std::string type;
  // element unit (e.q. m/s^2, mm/m^2)
  std::string unit;  
};
// ~~ stormy::meteo::Element
}}
