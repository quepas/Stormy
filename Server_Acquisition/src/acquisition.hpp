#pragma once

#include "db_mongo_handler.hpp"
#include "py_parse_script.hpp"
#include "py_script_storage.hpp"
#include "settings.hpp"
#include "common/net_util.hpp"
#include "common/util.h"
#include "common/util_task_template.hpp"
#include "common/util_task_scheduler.hpp"

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
      std::cout << "Acquired: " << map.size() << " itmes." << std::endl;
    }
    /*
    auto data = py_parser_->ParseFromStation(station_);
    database_handler_.InsertMeasurement(data);
    */
  }
};

}
// ~~ stormy::
