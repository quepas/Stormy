#pragma once

#include <ctime>
#include <cstdint>
#include <string>

namespace stormy {
  namespace common {
    namespace entity {

struct Station
{
  std::string uid;
  std::string name;
  std::string url;
  uint32_t refresh_time;
  std::tm last_update;

  // non-entity fields
  std::string parser_class;
};
// ~~ stormy::common::entity::Station
}}}
