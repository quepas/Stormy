#include <iostream>

#include "py_executor.h"
#include "acquisition_config_metrics.h"
#include "acquisition_config_station.h"
#include "MongoDBHandler.h"
#include "../../common/util.h"
#include "MeteoData.h"
#include "rest_service.h"
#include "acquisition_scheduler.h"

#include "../../common/db_expiration_engine.h"

#include <sstream>

using namespace Stormy;
using namespace Meteo;

int main(int argc, char** argv)
{
	std::cout << "++++++++++++++++ Acquisition Module ++++++++++++++++" << std::endl;
	PY_EXECUTOR_INIT();
	stormy::acquisition::config::Station meteoStationsCfg("config/meteo_stations_config.yaml");
	stormy::acquisition::config::Metrics meteoTypeCfg("config/meteo_data_type_config.yaml");

	MongoDBHandler& dbHandler = MongoDBHandler::get();
  dbHandler.set_expiration_seconds(3600 * 72);
	dbHandler.clearStationsData();
	dbHandler.insertStationsData(meteoStationsCfg.getConfiguration());
	dbHandler.clearTypesData();
	dbHandler.insertTypesData(meteoTypeCfg.Configuration());
	
	stormy::acquisition::Scheduler acqSecheduler;
	acqSecheduler.Schedule(meteoStationsCfg.getConfiguration());

  stormy::common::db::expiration::Engine expiration_engine(dbHandler);
  expiration_engine.ScheduleEverySeconds(3600);

	stormy::rest::Service httpServer;
	httpServer.run(argc, argv);  
  
	getchar();
}