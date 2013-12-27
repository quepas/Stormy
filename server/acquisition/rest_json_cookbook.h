#pragma once

#include "../../common/entity_metrics.h"
#include "../../common/entity_station.h"
#include "../../common/entity_measurement.h"

#include <vector>

namespace stormy {
  namespace rest {
    namespace json {
      namespace cookbook {

std::string PrepareStation(common::entity::Station station);
std::string PrepareStations(
  const std::vector<common::entity::Station>& stations);

std::string PrepareCombinedMeasurement(
  const std::vector<common::entity::Measurement>& measurement);
std::string PrepareCombinedMeasurements(
  const std::vector<common::entity::Measurement>& measurements);

std::string PrepareMetric(common::entity::Metrics metrics);
std::string PrepareMetrics(const std::vector<common::entity::Metrics>& metrics_vec);

std::string PrepareMeasurement(
  const std::vector<common::entity::Measurement>& measurement);
std::string PrepareMeasurements(
  const std::vector<common::entity::Measurement>& measurements);

std::string PrepareInfo(
  const std::string& server_type, 
  const std::vector<common::entity::Metrics>& metrics_vec);

static std::string wrapAsJSONString(std::string label);
// ~~ stormy::rest::json::Cookbook
}}}}
