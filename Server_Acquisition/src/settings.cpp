#include "settings.hpp"

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Util/JSONConfiguration.h>

using Poco::Exception;
using Poco::Logger;
using Poco::Util::JSONConfiguration;
using std::string;
using std::to_string;

namespace stormy {

StationSettings LoadStationSettings(const string& file_path)
{
  StationSettings settings;
  try {
    JSONConfiguration config(file_path);
    unsigned int idx = 0;
    string stations_key;
    while (config.has(stations_key = "stations[" + to_string(idx++) + "]")) {
      StationSetting entry;
      entry.name = config.getString(stations_key + ".name");
      entry.url = config.getString(stations_key + ".url");
      entry.parse_script = config.getString(stations_key + ".read_script");
      entry.update_time = config.getUInt(stations_key + ".update_time");
      settings.push_back(entry);
    }
  }
  catch (const Exception& exception) {
    Logger::get("main").error(exception.displayText());
  }
  return settings;
}

MetricsSettings LoadMetricsSettings(const string& file_path)
{

}

}
// ~~ stormy::
