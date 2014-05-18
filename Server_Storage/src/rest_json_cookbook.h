#pragma once

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "aggregation_entity_period.h"
#include "aggregation_entity_operation.h"
#include "aggregation_entity_aggregate.h"

#include <ctime>
#include <map>
#include <string>
#include <vector>

namespace stormy {
  namespace rest {
    namespace json {

class Cookbook 
{
public:
  static std::string PrepareServerInfo(
    const std::vector<common::entity::Metrics>& metrics, 
    const std::vector<aggregation::entity::Period>& periods);
  static std::string PrepareBadResponse(const std::string& uri);
  static std::string PrepareStation(const common::entity::Station& station);
  static std::string PrepareStations(
    const std::vector<common::entity::Station>& stations);
  static std::string PrepareTypedMeasurement(
    const std::map<std::time_t, std::string>& measurements);
  static std::string PrepareEmpty();  

  // api: /aggregate/:station_uid/:period_name/:timestamp
  static std::string PrepareAggregateSets(
    const std::map<std::time_t, std::vector<aggregation::entity::Aggregate>>& aggregate_sets);
  static std::string PrepareAggregateSet(
    std::time_t time,
    const std::vector<aggregation::entity::Aggregate>& aggregate_set);

  // api: /period
  static std::string PreparePeriods(
    const std::vector<aggregation::entity::Period>& periods);
  // api: /operation
  static std::string PrepareOperations(
    const std::vector<aggregation::entity::Operation>& operations);
private:
  Cookbook() {};
  Cookbook(const Cookbook&) {}
  Cookbook& operator=(const Cookbook&) {}  

  static std::string WrapAsJSONString(const std::string& text);  
  static std::string PrepareMetricsSequence(const std::vector<common::entity::Metrics>& metrics);
  static std::string PreparePeriodSequence(const std::vector<aggregation::entity::Period>& periods);

  static std::map<std::string, std::vector<aggregation::entity::Aggregate>> 
    ConvertAggregateSetToCodeAggregateSet(
      const std::vector<aggregation::entity::Aggregate>& aggregate_set);
};
// ~~ stormy::rest::json::Mapper
}}}