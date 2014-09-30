#pragma once

#include "meteo_element.hpp"

#include <ctime>
#include <string>
#include <vector>

namespace stormy {

struct DateTimeUTC
{
  // Seconds from UNIX-epoch UTC
  std::time_t epoch_time;
  // Time zone differential
  int time_zone;
};

struct MeteoDataEntry
{
  std::string element_id;
  double data;
  std::string str_data;
};

struct MeteoData
{
  DateTimeUTC datetime;
  std::vector<MeteoDataEntry> entries;
  std::string station_id;
};

}
// ~~ stormy::MeteoData
