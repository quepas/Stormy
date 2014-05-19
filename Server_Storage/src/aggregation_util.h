#pragma once

#include "entity_measurement.h"

#include <ctime>
#include <map>
#include <string>
#include <vector>

namespace stormy {
  namespace aggregation {
    namespace util {

std::map<std::string, std::vector<common::entity::Measurement>>
  ConvertMeasureSetsToMetricsSets(
    const std::map<std::time_t, std::vector<common::entity::Measurement>>& measure_sets);
// ~~ stormy::aggregation::util
}}}
