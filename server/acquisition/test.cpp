#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"
#include "MeteoStationsCfg.h"
#include "MongoDBHandler.h"
#include "../../common/MeteoStation.h"
#include "Utils.h"

#include "HttpServer.h"

#include <sstream>

int main(int argc, char** argv) 
{	
	std::cout << "++++++++++++++++ Acquisition Module Test ++++++++++++++++" << std::endl;	
	Py_ExecutorInit();	
	Stormy::PyParserWrapper* meteoBParser = new Stormy::PyParserWrapper("MeteoBParser");
	Stormy::MeteoStationsCfg* meteoStationsCfg = new Stormy::MeteoStationsCfg("config/meteo_stations_config.yaml");
	Stormy::MongoDBHandler* dbHandler = new Stormy::MongoDBHandler("localhost");

	dbHandler -> clearStationsData();
	dbHandler -> insertStationsData(meteoStationsCfg->getConfiguration());

	std::vector<Stormy::MeteoStation*> stations = meteoStationsCfg->getConfiguration();
	for(auto it = stations.begin(); it != stations.end(); ++it)
	{
		std::cout << "=== Parsing from station " << (*it) -> name 
			<< " ===\n\t(" << (*it) -> url << ")" << std::endl;
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		dbHandler -> insertMeteoData(meteoBParser -> parseFromURL((*it) -> url));
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	Stormy::HttpServer httpServer;
	return httpServer.run(argc, argv);
}