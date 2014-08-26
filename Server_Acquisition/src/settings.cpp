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
using std::string;
using std::to_string;

namespace stormy {

static Logger& logger_ = Logger::get("main");

StationSettings LoadStationSettings(const string& file_path)
{
  StationSettings settings;
  try {
    JSONConfiguration config(file_path);
    unsigned int idx = 0;
    string stations_key;
    while (config.has(stations_key = format("stations[%u]", idx++))) {
      try {
        StationSetting entry;
        entry.name = config.getString(stations_key + ".name");
        entry.url = config.getString(stations_key + ".url");
        entry.parse_script = config.getString(stations_key + ".read_script");
        entry.update_time = config.getUInt(stations_key + ".update_time");
        settings.push_back(entry);
      }
      catch (const NotFoundException& exception) {
        logger_.error(
          "Skipping entry "
          + format(stations_key, idx - 1)
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

MetricsSettings LoadMetricsSettings(const string& file_path)
{
  MetricsSettings settings;
  try {
    JSONConfiguration config(file_path);
    unsigned int idx = 0;
    string metrics_key;
    while (config.has(metrics_key = format("metrics[%u]", idx++))) {
      MetricsSetting entry;
      try {
        entry.name = config.getString(metrics_key + ".name");
        entry.type = config.getString(metrics_key + ".type");
        entry.unit = config.getString(metrics_key + ".unit");
        string labels_key;
        unsigned int label_idx = 0;
        while (config.has(labels_key = format(metrics_key + ".labels[%u]", label_idx++))) {
          entry.labels.push_back(config.getString(labels_key));
        }
      }
      catch (const NotFoundException& exception) {
        logger_.error(
          "Skipping entry "
          + format(metrics_key, idx - 1)
          + ": "
          + exception.displayText());
      }
      string format_key = metrics_key + ".format";
      if (config.has(format_key)) {
        entry.format = config.getString(format_key);
      }
      string is_meteo_key = metrics_key + ".is_meteo";
      if (config.has(is_meteo_key)) {
        entry.is_meteo = config.getBool(is_meteo_key);
      }
      settings.push_back(entry);
    }
  }
  catch (const Exception& exception) {
    logger_.error(exception.displayText());
  }
  return settings;
}

void SetupLoggers()
{
  AutoPtr<WindowsColorConsoleChannel> channel(new WindowsColorConsoleChannel);
  Logger::root().setChannel(channel);
  logger_.setChannel(channel);
}

}
// ~~ stormy::
