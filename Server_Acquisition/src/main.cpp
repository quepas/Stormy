#ifdef STORMY_OLD

#include "acquisition_scheduler.h"
#include "db_expiration_engine.h"
#include "db_mongo_handler.h"
#include "net_http_server.h"
#include "py_script_storage.hpp"
#include "settings.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
  std::cout << "==== Aqcuisition started. ====" << std::endl;

  stormy::SetupLoggers();
  auto station_settings = stormy::LoadStationSettings("config/meteo_stations.json");
  auto metrics_settings = stormy::LoadMetricsSettings("config/meteo_metrics.json");

  // register Python-based parse scripts
  stormy::PyScriptStorage central_storage;
  central_storage.Push("AGHReader", new stormy::PyParseScript("./AGHReader.py"));
  central_storage.Push("ECOCLIMA_MeteoParser", new stormy::PyParseScript("./ECOCLIMA_MeteoParser.py"));
  central_storage.Push("StacjameteoReader", new stormy::PyParseScript("./StacjameteoReader.py"));

  auto& db_handler = stormy::db::MongoHandler::get();
  db_handler.set_expiration_seconds(3600 * 72);
  db_handler.clearStationsData();
  db_handler.InsertStationsData(station_settings);
  db_handler.RemoveMetrics();
  db_handler.InsertMetrics(metrics_settings);

  stormy::acquisition::Scheduler acqSecheduler(central_storage);
  acqSecheduler.Schedule(station_settings);

  /*stormy::common::db::expiration::Engine expiration_engine(dbHandler);
  expiration_engine.ScheduleEverySeconds(3600); */

  stormy::net::HTTPServer httpServer(db_handler, 8080);
  httpServer.run(argc, argv);

  getchar();
}

#endif