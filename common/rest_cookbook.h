#pragma once

#include "entity_station.h"

#include <string>
#include <vector>

namespace stormy {
  namespace common {
    namespace rest {
      namespace cookbook {

std::string PrepareError(
  std::string error = "Bad request", 
  std::string msg = "");

std::string PrepareStationUIDs(
  const std::vector<entity::Station>& stations);
std::string PrepareStationInfo(
  const entity::Station& station);

template<typename T>
std::string WrapAsString(const T value);
template<typename T>
std::string WrapAsList(const T values);

// ~~ stormy::common::rest::cookbook
}}}}