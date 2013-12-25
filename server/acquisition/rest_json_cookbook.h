#pragma once

#include "MeteoData.h"

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

std::string PrepareMetric(Stormy::TypePtr type);
std::string PrepareMetrics(const Stormy::TypePtrVector& type);

std::string PrepareMeasurement(
  Stormy::MeasurementPtr measurement);
std::string PrepareMeasurements(
  const Stormy::MeasurementPtrVector& measurements);

std::string PrepareInfo(
  const std::string& server_type, 
  const Stormy::TypePtrVector& type);

static std::string wrapAsJSONString(std::string label);
// ~~ stormy::rest::json::Cookbook
}}}}
