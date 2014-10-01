#pragma once

#include <common/meteo_data.hpp>
#include <common/station_data.hpp>

#include <string>
#include <vector>

namespace stormy {

std::vector<StationData> LoadStationsFromFile(const std::string& file_path);
std::vector<MeteoElement> LoadMeteoElementsFromFile(const std::string& file_path);
void SetupLoggers();

}
// ~~ stormy::
