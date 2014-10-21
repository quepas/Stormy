#pragma once

#include "common/meteo_data.hpp"
#include "common/station_data.hpp"
#include "entity_station.h"
#include "entity_measurement.h"
#include "entity_metrics.h"

//#include <json/json.h>
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
std::string PrepareStationUIDs(const std::vector<StationData>& stations);
// api: /station/:station_uid
std::string PrepareStationInfo(const StationData& station);

// api: /meteo, /export, /aggregate
std::string PrepareStationUIDsWithAny(
  const std::vector<StationData>& stations,
  std::string data_key);

// api: /meteo/:station_uid
std::string PrepareMeteoTimestamps(const std::vector<std::time_t>& timestamps);
// api: /meteo/:station_uid/:timestamp
std::string PrepareMeteoSets(
  const std::map<std::time_t,
  std::vector<entity::Measurement>>& ts_measure_sets_map);

// api: /meteo_element
std::string PrepareMeteoElementIds(const std::vector<MeteoElement>& metrics_vec);
// api: /meteo_element/:element_id
std::string PrepareMeteoElementDetails(const MeteoElement& metrics);

// api: /aggregate/:station_uid
std::string PreparePeriodNamesWithAny(
  const std::vector<std::string>& period_names,
  std::string data_key);
// api: /aggregate/:station_uid/:period_name
std::string PrepareStationPeriodStartTimes(const std::vector<std::tm>& start_times);

// api: /info
std::string PrepareServerInfo(std::string name, std::string type, std::string timezone);

//std::string ToStyledString(const Json::Value& value);

}}}}
// ~~ stormy::common::rest::cookbook
