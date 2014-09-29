#pragma once

#include <common/station_data.hpp>

#include <string>
#include <vector>

namespace stormy {

struct MetricsSetting
{
  std::string name;
  std::string type;
  std::string unit;
  std::string format;
  std::vector<std::string> labels;
  bool is_meteo;
};

typedef std::vector<StationData> StationSettings;
typedef std::vector<MetricsSetting> MetricsSettings;

StationSettings LoadStationSettings(const std::string& file_path);
MetricsSettings LoadMetricsSettings(const std::string& file_path);
void SetupLoggers();

}
// ~~ stormy::
