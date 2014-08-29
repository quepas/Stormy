#pragma once

#include "common/entity_measurement.h"

#include <ctime>
#include <map>
#include <vector>
#include <string>

namespace stormy {
  namespace rest {
    namespace csv {

// api: /export/:station_uid + query (from, to, metrics)
std::string PrepareMeteo(  
  const std::map<std::time_t, std::vector<common::entity::Measurement>>& measure_sets,
  const std::vector<std::string>& metrics);

std::string PrepareHeader(const std::vector<std::string>& metrics);
// ~~ stormy::rest::csv
}}}
