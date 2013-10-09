#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"
#include "MeteoStationsCfg.h"
#include "../../common/MeteoStation.h"

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

int main() 
{	
	std::cout << "Acquisition Module Test" << std::endl;
	Py_ExecutorInit();	
	Stormy::PyParserWrapper* meteoBParser = new Stormy::PyParserWrapper("MeteoBParser");
	Stormy::MeteoStationsCfg* meteoStationsCfg = new Stormy::MeteoStationsCfg("config/meteo_stations_config.yaml");

	std::vector<Stormy::MeteoStation*> stations = meteoStationsCfg->getConfiguration();
	for(auto it = stations.begin(); it != stations.end(); ++it)
	{
		meteoBParser->parseFromURL((*it)->url);
	}

	/*for(int i = 20; i < 40; ++i)
	{		
		meteoBParser -> parseFromURL(std::string("http://stacjameteo.pl/dane/index.php?stacja=") + lexical_cast<std::string>(i));
	}*/	

	getchar();
}