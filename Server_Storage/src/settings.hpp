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

struct DatabaseSetting
{
  std::string name;
  std::string host;
  unsigned port;
  std::string user;
  std::string password;
};

std::vector<RemoteServerSetting> LoadRemoteServerSettings(const std::string& file_path);
std::vector<AggregateSetting> LoadAggregateSettings(const std::string& file_path);
DatabaseSetting LoadDatabaseSetting(const std::string& file_path);

std::string ToString(const RemoteServerSetting& setting);
std::string ToString(const AggregateSetting& setting);
std::string ToString(const DatabaseSetting& setting);
std::string ToPostgreString(const DatabaseSetting& setting);
void SetupLoggers();

}
// ~~ stormy::
