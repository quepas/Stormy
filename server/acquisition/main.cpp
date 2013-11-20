#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"
#include "StationConfiguration.h"
#include "MongoDBHandler.h"
#include "../../common/Utils.h"
#include "MeteoUtils.h"
#include "MeteoData.h"
#include "HttpServer.h"
#include "AcquisitionScheduler.h"

#include <sstream>

using namespace Stormy;
using namespace Meteo;

int main(int argc, char** argv)
{
	std::cout << "++++++++++++++++ Acquisition Module ++++++++++++++++" << std::endl;
	Py_ExecutorInit();
	StationConfiguration meteoStationsCfg("config/meteo_stations_config.yaml");
	TypeConfiguration meteoTypeCfg("config/meteo_data_type_config.yaml");

	MongoDBHandler& dbHandler = MongoDBHandler::get();
	dbHandler.clearStationsData();
	dbHandler.insertStationsData(meteoStationsCfg.getConfiguration());
	dbHandler.clearTypesData();
	dbHandler.insertTypesData(meteoTypeCfg.getConfiguration());
	
	AcquisitionScheduler acqSecheduler;
	acqSecheduler.scheduleManyAcquisition(meteoStationsCfg.getConfiguration());

	Stormy::HttpServer httpServer;
	return httpServer.run(argc, argv);
}