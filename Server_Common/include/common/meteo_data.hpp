#pragma once

#include "station_data.hpp"

#include <string>

namespace stormy {

struct MeteoElement
{
  std::string id;
  double value;
  std::string str_value;
};

struct MeteoData
{
  std::string datetime;
  MeteoElement* elements;
  unsigned int num_elements;
  StationData station;
};

}
// ~~ stormy::MeteoData
