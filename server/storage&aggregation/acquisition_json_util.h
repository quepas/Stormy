#pragma once

#include <string>
#include <vector>

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"

namespace stormy {
  namespace acquisition {
    namespace json {

std::vector<common::entity::Station> 
  ExtractStations(std::string content);
std::vector<common::entity::Measurement>	
  ExtractMeasurements(std::string content);
std::vector<common::entity::Metrics> 
  ExtractMetrics(std::string content);
// ~~ stormy::acquisition::json::util
}}}