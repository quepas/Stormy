#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"
#include "MeteoStationsCfg.h"
#include "MeteoDataTypeEquivalentCfg.h"
#include "MongoDBHandler.h"
#include "../../common/MeteoStation.h"

#include "HttpServer.h"

#include <sstream>

template<typename Out, typename In>
Out lexical_cast(In in)
{
	std::stringstream ss;
	Out out;

	ss << in;
	ss >> out;

	return out;
};

int main(int argc, char** argv) 
{	
	std::cout << "++++++++++++++++ Acquisition Module Test ++++++++++++++++" << std::endl;	
	Py_ExecutorInit();	
	Stormy::PyParserWrapper* meteoBParser = new Stormy::PyParserWrapper("MeteoBParser");
	Stormy::MeteoStationsCfg* meteoStationsCfg = new Stormy::MeteoStationsCfg("config/meteo_stations_config.yaml");
	Stormy::MeteoDataTypeEquivalentCfg* meteoDataTypeEquivalentCfg = new Stormy::MeteoDataTypeEquivalentCfg("config/meteo_data_type_equivalents.yaml");
	Stormy::MongoDBHandler* dbHandler = new Stormy::MongoDBHandler("localhost");

	dbHandler->setDbAndCollection("test", "person");
	dbHandler->insertMeteoData(nullptr);
	std::vector<Stormy::MeteoStation*> stations = meteoStationsCfg->getConfiguration();
	for(auto it = stations.begin(); it != stations.end(); ++it)
	{
		std::cout << "=== Parsing from station " << (*it) -> name 
			<< " ===\n\t(" << (*it) -> url << ")" << std::endl;
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		meteoBParser -> parseFromURL((*it) -> url);
		std::cout << "--------------------------------------------------------------------------" << std::endl;
		std::cout << std::endl;
	}

	Stormy::HttpServer httpServer;
	return httpServer.run(argc, argv);
}