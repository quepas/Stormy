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

};

typedef std::vector<StationSetting> StationSettings;
typedef std::vector<MetricsSetting> MetricsSettings;

StationSettings LoadStationSettings(const std::string& file_path);
MetricsSettings LoadMetricsSettings(const std::string& file_path);

}
// ~~ stormy::
