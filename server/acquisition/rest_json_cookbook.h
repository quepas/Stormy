#pragma once

#include "../../common/entity_metrics.h"
#include "MeteoData.h"

#include <vector>

namespace stormy {
  namespace rest {
    namespace json {
      namespace cookbook {

std::string PrepareStation(Stormy::StationPtr station);
std::string PrepareStations(const Stormy::StationPtrVector& stations);

std::string PrepareCombinedMeasurement(
  Stormy::MeasurementPtr measurement);
std::string PrepareCombinedMeasurements(
  const Stormy::MeasurementPtrVector& measurements);

std::string PrepareMetric(common::entity::Metrics metrics);
std::string PrepareMetrics(const std::vector<common::entity::Metrics>& metrics_vec);

std::string PrepareMeasurement(
  Stormy::MeasurementPtr measurement);
std::string PrepareMeasurements(
  const Stormy::MeasurementPtrVector& measurements);

std::string PrepareInfo(
  const std::string& server_type, 
  const std::vector<common::entity::Metrics>& metrics_vec);

static std::string wrapAsJSONString(std::string label);
// ~~ stormy::rest::json::Cookbook
}}}}
