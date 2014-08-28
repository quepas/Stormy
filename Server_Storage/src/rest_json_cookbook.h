#pragma once

#include "aggregation_entity_period.h"
#include "aggregation_entity_operation.h"
#include "aggregation_entity_aggregate.h"

#include <map>
#include <string>
#include <vector>

namespace stormy {
  namespace rest {
    namespace json {

class Cookbook
{
public:
  // api: /aggregate/:station_uid/:period_name/:timestamp
  static std::string PrepareAggregateSets(
    const std::map<std::time_t, std::vector<aggregation::entity::Aggregate>>& aggregate_sets);

  // api: /period
  static std::string PreparePeriods(
    const std::vector<aggregation::entity::Period>& periods);
  // api: /operation
  static std::string PrepareOperations(
    const std::vector<aggregation::entity::Operation>& operations);

  Cookbook(const Cookbook&) = delete;
  Cookbook& operator=(const Cookbook&) = delete;
private:
  Cookbook() {};
};
// ~~ stormy::rest::json::Mapper
}}}