#pragma once

#include <ctime>
#include <string>
#include <vector>

namespace stormy {

struct RemoteServerSetting
{
  std::string id;
  std::string name;
  std::string host;
  unsigned port;
  std::time_t update_time;
};

struct AggregateSetting
{
  std::string id;
  std::string pq_interval;
  std::size_t update_time;
  bool is_enable;
};

std::vector<RemoteServerSetting> LoadRemoteServerSettings(const std::string& file_path);
std::vector<AggregateSetting> LoadAggregateSettings(const std::string& file_path);

std::string ToString(const RemoteServerSetting& setting);
std::string ToString(const AggregateSetting& setting);
void SetupLoggers();

}
// ~~ stormy::
