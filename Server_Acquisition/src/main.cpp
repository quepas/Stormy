#ifdef STORMY_OLD

#include <iostream>
#include "acquisition_config_metrics.h"
#include "acquisition_config_station.h"
#include "db_mongo_handler.h"
#include "rest_service.h"
#include "acquisition_scheduler.h"

#include "py_parse_script.hpp"
#include "py_script_storage.hpp";
#include "util.h"
#include "db_expiration_engine.h"

#include <sstream>

int main(int argc, char** argv)
{
  std::cout << "==== Aqcuisition started. ====" << std::endl;
  stormy::acquisition::config::Station meteoStationsCfg("config/meteo_stations.yaml");
  stormy::acquisition::config::Metrics meteoTypeCfg("config/meteo_data_type_config.yaml");

  // register Python-based parse scripts
  stormy::PyScriptStorage central_storage;
  central_storage.Push("AGHReader", new stormy::PyParseScript("./AGHReader.py"));
  central_storage.Push("ECOCLIMA_MeteoParser", new stormy::PyParseScript("./ECOCLIMA_MeteoParser.py"));
  central_storage.Push("StacjameteoReader", new stormy::PyParseScript("./StacjameteoReader.py"));

  auto& dbHandler = stormy::db::MongoHandler::get();
  dbHandler.set_expiration_seconds(3600 * 72);
  dbHandler.clearStationsData();
  dbHandler.insertStationsData(meteoStationsCfg.getConfiguration());
  dbHandler.RemoveMetrics();
  dbHandler.InsertMetrics(meteoTypeCfg.Configuration());

  stormy::acquisition::Scheduler acqSecheduler(central_storage);
  acqSecheduler.Schedule(meteoStationsCfg.getConfiguration());

  /*stormy::common::db::expiration::Engine expiration_engine(dbHandler);
  expiration_engine.ScheduleEverySeconds(3600);*/

  stormy::rest::Service httpServer;
  httpServer.run(argc, argv);

  getchar();
}

#endif