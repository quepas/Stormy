#pragma once

#include <string>
#include <cstdint>

namespace stormy {
  namespace common {
    namespace entity {

struct Measurement
{
  uint64_t id;
  std::string code;
  std::string station_uid;
  std::string value_text;
  double value_number;
  std::tm timestamp;

  // non-entity fields
  bool is_numeric;
};
// ~~ stormy::common::entity::Measurement
}}}