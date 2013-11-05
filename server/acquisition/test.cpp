#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"
#include "StationConfiguration.h"
#include "MongoDBHandler.h"
#include "Utils.h"
#include "MeteoUtils.h"
#include "MeteoData.h"
#include "HttpServer.h"
#include "AcquisitionScheduler.h"

#include <sstream>

using namespace Stormy;
using namespace Meteo;

int main(int argc, char** argv)
{
	std::cout << "++++++++++++++++ Acquisition Module Test ++++++++++++++++" << std::endl;
	Py_ExecutorInit();
	StationConfiguration* meteoStationsCfg = new StationConfiguration("config/meteo_stations_config.yaml");
	MongoDBHandler* dbHandler = new MongoDBHandler("localhost");

	dbHandler -> clearStationsData();
	dbHandler -> insertStationsData(meteoStationsCfg->getConfiguration());
	AcquisitionScheduler* acqSecheduler = new AcquisitionScheduler();
	acqSecheduler -> scheduleManyAcquisition(meteoStationsCfg->getConfiguration());

	Stormy::HttpServer httpServer;
	return httpServer.run(argc, argv);
}