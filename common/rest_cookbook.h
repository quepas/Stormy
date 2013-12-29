#pragma once

#include "entity_station.h"
#include "entity_measurement.h"
#include "entity_metrics.h"

#include <ctime>
#include <string>
#include <map>
#include <vector>

namespace stormy {
  namespace common {
    namespace rest {
      namespace cookbook {

std::string PrepareError(
  std::string error = "Bad request", 
  std::string msg = "");

// api: /station
std::string PrepareStationUIDs(
  const std::vector<entity::Station>& stations);
// api: /station/:station_uid
std::string PrepareStationInfo(
  const entity::Station& station);

// api: /meteo
std::string PrepareMeteoCountPerStation(
  const std::map<std::string, uint32_t>& uid_size_map);
// api: /meteo/:station_uid
std::string PrepareMeteoTimestamps(
  const std::vector<entity::Measurement>& measurements);
std::string PrepareMeteoTimestamps(
  const std::vector<std::time_t>& timestamps);
// api: /meteo/:station_uid/:timestamp
std::string PrepareMeteoSets(
  const std::map<std::time_t, std::vector<entity::Measurement>>& ts_measure_sets_map);
std::string PrepareMeteoSet(std::time_t ts, const std::vector<entity::Measurement>& measure_set);

// api: /metrics
std::string PrepareMetricsCodes(
  const std::vector<entity::Metrics>& metrics_vec);
// api: /metrics/:metrics_code
std::string PrepareMetricsInfo(const entity::Metrics& metrics);

template<typename T>
std::string WrapAsString(const T value);
template<typename T>
std::string WrapAsList(const T values);
std::string WrapAsJSON(std::string content);

// ~~ stormy::common::rest::cookbook
}}}}