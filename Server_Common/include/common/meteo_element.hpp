#pragma once

#include <string>
#include <vector>

namespace stormy {

struct MeteoElement
{
  std::string id;
  std::vector<std::string> labels;
  std::string type;
};

}
// ~~ stormy::MeteoElement
