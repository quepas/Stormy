#ifdef STORMY_OLD

#include "common/db_expiration_engine.h"
#include "common/util_task_scheduler.hpp"
#include "db_mongo_handler.hpp"
#include "net_http_server.hpp"
#include "py_script_storage.hpp"
#include "acquisition.hpp"
#include "settings.hpp"
#include "common/util.h"

#include <string>
#include <Poco/Logger.h>

int main(int argc, char** argv)
{
  stormy::SetupLoggers();
  auto& logger = Poco::Logger::get("main");
  logger.information("==== Aqcuisition started. ====");

  auto station_settings = stormy::LoadStationSettings("config/meteo_stations.json");
  auto metrics_settings = stormy::LoadMetricsSettings("config/meteo_metrics.json");

  // register Python-based parse scripts
  stormy::PyScriptStorage central_storage;
  central_storage.Push("AGHReader", new stormy::PyParseScript("./AGHReader.py"));
  //central_storage.Push("ECOCLIMA_MeteoParser", new stormy::PyParseScript("./ECOCLIMA_MeteoParser.py"));
  central_storage.Push("StacjameteoReader", new stormy::PyParseScript("./StacjameteoReader.py"));

  auto& db_handler = stormy::db::MongoHandler::get();
  db_handler.set_expiration_seconds(3600 * 72);
  db_handler.ClearStations();
  db_handler.InsertStations(station_settings);
  db_handler.ClearMetrics();
  db_handler.InsertMetrics(metrics_settings);

  stormy::util::TaskScheduler acq_scheduler;
  for (auto station : station_settings) {
    auto parse_script = central_storage.Fetch(station.parse_script);
    if (parse_script != nullptr) {
      stormy::AcquisitionContext task_context = { 
        0, 
        stormy::common::SecondsToMiliseconds(station.update_time),
        station, 
        db_handler, 
        parse_script 
      };
      acq_scheduler.Schedule<stormy::AcquisitionTask>(task_context);
    }
    else {
      logger.error("Missing parse script with class: " + station.parse_script);
    }
  }

  stormy::common::db::expiration::Engine expiration_engine(db_handler);
  expiration_engine.ScheduleEverySeconds(3600);

  stormy::net::AcquisitionHttpServer http_server({db_handler}, 8080);
  http_server.run(argc, argv);

  getchar();
}

#endif