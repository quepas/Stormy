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

#include <string>
#include <Poco/Logger.h>

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
    logger.information("[acquisition/Task] Acquire weather from " + common::MD5(context.station.url));
    std::string website_content = net::FetchWebsite(context.station.url);

    if (!website_content.empty()) {
      auto map = (*context.script)(website_content);
      logger.information("Acquired: " + std::to_string(map.size()) + " itmes.");

      auto metrics = context.database_handler.GetMetrics();
      logger.information("Metrics size: " + std::to_string(metrics.size()));
      db::MeteoData meteo_data;
      for (auto& entry : map) {
        std::string key = entry.first;
        std::string value = entry.second;
        for (auto& m_entry : metrics) {
          if (m_entry.code == key || m_entry.equivalents.find(key) != std::string::npos) {
            common::entity::Measurement measure;
            measure.code = key;
            measure.station_uid = context.station.id;
            if (m_entry.type == "number") {
              measure.value_number = std::stod(value);
            }
            else {
              measure.value_text = value;
            }
            meteo_data.push_back(measure);
          }
        }
      }
      context.database_handler.InsertMeteo(meteo_data);
    }
  }
};

}
// ~~ stormy::
