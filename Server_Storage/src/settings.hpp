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

std::vector<RemoteServerSetting> LoadRemoteServerSettings(const std::string& file_path);
std::string ToString(const RemoteServerSetting& setting);
void SetupLoggers();

}
// ~~ stormy::
