#pragma once

#include <string>
#include <vector>

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"

namespace stormy {
  namespace acquisition {
    namespace json {

common::entity::Station ExtractStation(std::string json_response);

std::vector<common::entity::Measurement>	
  ExtractMeasurements(std::string content);
common::entity::Metrics ExtractMetrics(std::string json_response);

std::vector<std::string> ExtractSimpleListElements(
  std::string json_response, 
  std::string list_key);
// ~~ stormy::acquisition::json::util
}}}