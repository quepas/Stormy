#include <iostream>

#include "py_executor.h"
#include "acquisition_config_metrics.h"
#include "acquisition_config_station.h"
#include "db_mongo_handler.h"
#include "../../common/util.h"
#include "rest_service.h"
#include "acquisition_scheduler.h"

#include "../../common/db_expiration_engine.h"

#include <sstream>

int main(int argc, char** argv)
{
  std::cout << "==== Aqcuisition started. ====" << std::endl;
  PY_EXECUTOR_INIT();
  stormy::acquisition::config::Station meteoStationsCfg("config/meteo_stations_config.yaml");
  stormy::acquisition::config::Metrics meteoTypeCfg("config/meteo_data_type_config.yaml");

  auto& dbHandler = stormy::db::MongoHandler::get();
  dbHandler.set_expiration_seconds(3600 * 72);
  dbHandler.clearStationsData();
  dbHandler.insertStationsData(meteoStationsCfg.getConfiguration());
  dbHandler.RemoveMetrics();
  dbHandler.InsertMetrics(meteoTypeCfg.Configuration());

  /*stormy::acquisition::Scheduler acqSecheduler;
  acqSecheduler.Schedule(meteoStationsCfg.getConfiguration());

  stormy::common::db::expiration::Engine expiration_engine(dbHandler);
  expiration_engine.ScheduleEverySeconds(3600);*/

  stormy::rest::Service httpServer;
  httpServer.run(argc, argv);

  getchar();
}