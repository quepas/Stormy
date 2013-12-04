#pragma once

#include <string>
#include <cstdint>

namespace stormy {
  namespace aggregate {
    namespace entity {

struct Aggregate
{
  uint64_t id;
  std::string station_uid;
  std::string metrics_code;
  std::string operation_name;
  std::string period_name;
  std::tm start_time;
  double value;
  uint16_t sample_number;
};
// ~~ stormy::aggregate::entity::Aggregate
}}}