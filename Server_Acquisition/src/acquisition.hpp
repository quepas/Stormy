#pragma once

#include "db_mongo_handler.hpp"
#include "py_parse_script.hpp"
#include "py_script_storage.hpp"
#include "settings.hpp"
#include "common/entity_measurement.h"
#include "common/net_util.hpp"
#include "common/util.h"
#include "common/util_task_template.hpp"
#include "common/util_task_scheduler.hpp"
#include "common/meteo_data.hpp"
#include "common/station_data.hpp"

#include <string>
#include <Poco/Logger.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>

namespace stormy {

static Poco::Logger& logger = Poco::Logger::get("main");

struct AcquisitionContext;
class AcquisitionAction;

typedef util::TaskTemplate<AcquisitionContext, AcquisitionAction> AcquisitionTask;

struct AcquisitionContext
{
  long delay;
  long interval;

  StationData station;
  db::MongoHandler& database_handler;
  PyParseScript* script;
};

class AcquisitionAction
{
public:
  static void run(AcquisitionContext context) {
    logger.information("[acq/Task] Acquire weather from " + context.station.id);
    std::string website_url = context.station.url;
    std::string website_content = net::FetchWebsite(website_url);

    if (website_content.empty()) {
      logger.warning("[acq/Task] Website " + website_url + " is empty.");
    }
    else {
      auto meteo_data = ExtractMeteoData(context, website_content);
      auto& db_handler = context.database_handler;
      if (!db_handler.IsMeteoExists(meteo_data)) {
        db_handler.InsertMeteo(meteo_data);
      }
    }
  }
private:
  static MeteoData ExtractMeteoData(AcquisitionContext context, std::string website_content) {
    auto raw_meteo = (*context.script)(website_content);
    auto metrics = context.database_handler.GetMetrics();
    MeteoData meteo_data;
    meteo_data.station_id = context.station.id;

    Poco::DateTime date_time;
    int station_timezone = common::HoursToSeconds(context.station.time_zone);
    for (auto& entry : raw_meteo) {
      std::string key = entry.first;
      std::string value = entry.second;
      for (auto& m_entry : metrics) {
        if (m_entry.code == key || m_entry.equivalents.find(key) != std::string::npos) {
          if (key == "datetime") {
            Poco::DateTimeParser parser;
            int time_zone;
            if (!parser.tryParse(value, date_time, time_zone)) {
              logger.error("[acq/Task] Wrong datetime. Skipping acquisition.");
              return meteo_data;
            }
            // TODO: fix DST/"Summer time" problem
            //station_timezone = time_zone;
          }
          else {
            MeteoDataEntry meteo_element;
            meteo_element.element_id = key;
            if (m_entry.type == "number") {
              meteo_element.data = std::stod(value);
            }
            else {
              meteo_element.str_data = value;
            }
            meteo_data.entries.push_back(meteo_element);
          }
        }
      }
    }
    date_time.makeUTC(station_timezone);
    meteo_data.datetime = { date_time.timestamp().epochTime(), context.station.time_zone };
    return meteo_data;
  }
};

}
// ~~ stormy::
