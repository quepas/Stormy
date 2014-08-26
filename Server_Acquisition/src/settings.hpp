#pragma once

#include <string>
#include <vector>

namespace stormy {

struct StationSetting
{
  std::string name;
  std::string url;
  std::string parse_script;
  unsigned int update_time;
};

struct MetricsSetting
{
  std::string name;
  std::string type;
  std::string unit;
  std::string format;
  std::vector<std::string> labels;
  bool is_meteo;
};

typedef std::vector<StationSetting> StationSettings;
typedef std::vector<MetricsSetting> MetricsSettings;

StationSettings LoadStationSettings(const std::string& file_path);
MetricsSettings LoadMetricsSettings(const std::string& file_path);
void SetupLoggers();

}
// ~~ stormy::
