#pragma once

#include <string>

namespace stormy {
  namespace common {
    namespace entity {

struct Metrics
{
  std::string code;
  std::string equivalents;
  std::string type;
  std::string unit;
  std::string format;

  // non-entity fields
  bool is_meteo;
};
// ~~ stormy::common::entity::Metrics
}}}