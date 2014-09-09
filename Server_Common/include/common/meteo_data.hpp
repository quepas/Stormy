#pragma once

#include "meteo_element.hpp"

#include <string>
#include <vector>

namespace stormy {

struct MeteoDataEntry
{
  std::string element_id;
  double data;
  std::string str_data;
};

struct MeteoData
{
  std::string datetime;
  std::vector<MeteoDataEntry> entries;
  std::string station_id;
};

}
// ~~ stormy::MeteoData
