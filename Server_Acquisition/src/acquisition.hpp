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

  StationSetting station;
  db::MongoHandler& database_handler;
  PyParseScript* script;
};

class AcquisitionAction
{
public:
  static void run(AcquisitionContext context) {
    logger.information("[acquisition/Task] Acquire weather from " + context.station.id);
    std::string website_content = net::FetchWebsite(context.station.url);

    if (!website_content.empty()) {
      auto map = (*context.script)(website_content);
      auto metrics = context.database_handler.GetMetrics();
      db::MeteoData_ old_meteo_data;
      MeteoData meteo_data;
      meteo_data.station_id = context.station.id;

      bool datetime_exists = false;
      Poco::DateTime date_time;
      for (auto& entry : map) {
        std::string key = entry.first;
        std::string value = entry.second;
        for (auto& m_entry : metrics) {
          if (m_entry.code == key || m_entry.equivalents.find(key) != std::string::npos) {
            if (key == "datetime") {
              datetime_exists = true;
              Poco::DateTimeParser parser;
              int time_zone;
              if (!parser.tryParse(value, date_time, time_zone)) {
                logger.error("[acquisition/Task] Wrong datetime. Skipping acquisition.");
              }
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
      if (datetime_exists) {
        meteo_data.datetime = Poco::DateTimeFormatter::format(date_time, "%Y-%m-%d %H:%M:%S");
      }
      context.database_handler.InsertMeteo(meteo_data);
    }
  }
};

}
// ~~ stormy::
