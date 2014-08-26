#ifdef STORMY_OLD

#include <iostream>
#include <string>
#include "db_mongo_handler.h"
#include "rest_service.h"
#include "acquisition_scheduler.h"

#include "py_script_storage.hpp"
#include "db_expiration_engine.h"

#include "settings.hpp"

int main(int argc, char** argv)
{
  std::cout << "==== Aqcuisition started. ====" << std::endl;

  auto station_settings = stormy::LoadStationSettings("config/meteo_stations.json");
  auto metrics_settings = stormy::LoadMetricsSettings("config/meteo_metrics.json");

  // register Python-based parse scripts
  stormy::PyScriptStorage central_storage;
  central_storage.Push("AGHReader", new stormy::PyParseScript("./AGHReader.py"));
  central_storage.Push("ECOCLIMA_MeteoParser", new stormy::PyParseScript("./ECOCLIMA_MeteoParser.py"));
  central_storage.Push("StacjameteoReader", new stormy::PyParseScript("./StacjameteoReader.py"));

  auto& dbHandler = stormy::db::MongoHandler::get();
  dbHandler.set_expiration_seconds(3600 * 72);
  dbHandler.clearStationsData();
  dbHandler.InsertStationsData(station_settings);
  dbHandler.RemoveMetrics();
  dbHandler.InsertMetrics(metrics_settings);

  stormy::acquisition::Scheduler acqSecheduler(central_storage);
  acqSecheduler.Schedule(station_settings);

  /*stormy::common::db::expiration::Engine expiration_engine(dbHandler);
  expiration_engine.ScheduleEverySeconds(3600); */

  stormy::rest::Service httpServer;
  httpServer.run(argc, argv);

  getchar();
}

#endif