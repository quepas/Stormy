#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <map>

#include "entity_station.h"
#include "entity_metrics.h"
#include "entity_measurement.h"

namespace stormy {
  namespace acquisition {
    namespace json {

common::entity::Station ExtractStation(std::string json_response);

std::map<std::time_t, std::vector<common::entity::Measurement>>
  ExtractMeasureSets(std::string content, std::string station_uid);
common::entity::Metrics ExtractMetrics(std::string json_response);

std::vector<std::string> ExtractSimpleListElements(
  std::string json_response, 
  std::string list_key);
// ~~ stormy::acquisition::json::util
}}}