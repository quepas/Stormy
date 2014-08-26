#include "settings.hpp"

#include <Poco/AutoPtr.h>
#include <Poco/Exception.h>
#include <Poco/Format.h>
#include <Poco/Logger.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/Util/JSONConfiguration.h>

using Poco::AutoPtr;
using Poco::Exception;
using Poco::format;
using Poco::NotFoundException;
using Poco::Logger;
using Poco::WindowsColorConsoleChannel;
using Poco::Util::JSONConfiguration;
using std::to_string;
using std::string;
using std::vector;

namespace stormy {

static Logger& logger_ = Logger::get("main");

vector<RemoteServerSetting> LoadRemoteServerSettings(const string& file_path)
{
  vector<RemoteServerSetting> settings;
  try {
    JSONConfiguration config(file_path);
    unsigned idx = 0;
    string remote_key;
    while (config.has(remote_key = format("remote_servers[%u]", idx++))) {
      try {
        RemoteServerSetting entry;
        entry.id = config.getString(remote_key + ".id");
        entry.name = config.getString(remote_key + ".name");
        entry.host = config.getString(remote_key + ".host");
        entry.port = config.getUInt(remote_key + ".port");
        entry.update_time = config.getUInt64(remote_key + ".update_time");
        settings.push_back(entry);
      }
      catch (const NotFoundException& exception) {
        logger_.error("Skipping entry "
          + format(remote_key, idx - 1)
          + ": "
          + exception.displayText());
      }
    }
  }
  catch (const Exception& exception) {
    logger_.error(exception.displayText());
  }
  return settings;
}

string ToString(const RemoteServerSetting& setting)
{
  return "[id=" + setting.id + ", "
    + "name=" + setting.name + ", "
    + "host=" + setting.host + ", "
    + "port=" + to_string(setting.port) + ", "
    + "update_time=" + to_string(setting.update_time) + "]";
}

void SetupLoggers()
{
  AutoPtr<WindowsColorConsoleChannel> channel(new WindowsColorConsoleChannel);
  Logger::root().setChannel(channel);
  logger_.setChannel(channel);
}

}
// ~~ stormy::
