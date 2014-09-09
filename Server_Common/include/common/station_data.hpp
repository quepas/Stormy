#pragma once

#include <ctime>
#include <string>

namespace stormy {

struct StationData
{
  std::string id;
  std::string name;
  std::string url;
  std::string parse_script;
  std::time_t update_time;
};

}
// ~~ stormy::StationData
