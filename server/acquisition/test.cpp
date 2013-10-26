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

#include <sstream>

using namespace Stormy;
using namespace Meteo;

int main(int argc, char** argv) 
{	
	std::cout << "++++++++++++++++ Acquisition Module Test ++++++++++++++++" << std::endl;	
	Py_ExecutorInit();	
	PyParserWrapper* meteoBParser = new PyParserWrapper("MeteoBParser");
	StationConfiguration* meteoStationsCfg = new StationConfiguration("config/meteo_stations_config.yaml");
	MongoDBHandler* dbHandler = new MongoDBHandler("localhost");

	dbHandler -> clearStationsData();
	dbHandler -> insertStationsData(meteoStationsCfg->getConfiguration());
	
	std::vector<Stormy::Meteo::Station*> stations = meteoStationsCfg->getConfiguration();
	for(auto it = stations.begin(); it != stations.end(); ++it)
	{
		std::cout << "=== Parsing from station " << (*it) -> name 
			<< " ===\n\t(" << (*it) -> url << ")" << std::endl;
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		//dbHandler -> insertMeteoData(meteoBParser -> parseFromURL((*it) -> url));
		meteoBParser -> parseFromURL((*it) -> url);
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	Stormy::HttpServer httpServer;
	return httpServer.run(argc, argv);	
}