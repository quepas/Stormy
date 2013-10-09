#include "MeteoStationsCfg.h"

using namespace Stormy;

Stormy::MeteoStationsCfg::MeteoStationsCfg( std::string filePath )
	:	configuration(std::vector<MeteoStation*>())
{
	if(!load(filePath))
	{
		std::cout << "Couldn't load file properly: " << filePath << std::endl;
		configuration.clear();
	}
}

Stormy::MeteoStationsCfg::~MeteoStationsCfg()
{
	configuration.clear();
}

bool Stormy::MeteoStationsCfg::reload()
{
	return false;
}

bool Stormy::MeteoStationsCfg::load(std::string filePath)
{
	YAML::Node root = YAML::LoadFile(filePath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{
		if(!checkIfFieldDefined(it, "url"))
		{
			std::cout << "Field url is not defined" << std::endl;
			return false;
		}
		if(!checkIfFieldDefined(it, "parserClass"))
		{
			std::cout << "Field parserClass is not defined" << std::endl;
			return false;
		}			
		if(!checkIfFieldDefined(it, "refreshTime"))
		{
			std::cout << "Field refreshTime is not defined" << std::endl;
			return false;
		}

		MeteoStation* meteoStation = new MeteoStation();
		meteoStation->url = (*it)["url"].as<std::string>();
		meteoStation->parserClass = (*it)["parserClass"].as<std::string>();
		meteoStation->refreshTime = (*it)["refreshTime"].as<double>();

		if(checkIfFieldDefined(it, "id"))
			meteoStation -> id = (*it)["id"].as<std::string>();
		if(checkIfFieldDefined(it, "name"))
			meteoStation -> name = (*it)["name"].as<std::string>();

		configuration.push_back(meteoStation);
	}
	return true;
}

bool MeteoStationsCfg::checkIfFieldDefined( YAML::iterator it, std::string field )
{
	return (*it)[field].IsDefined();
}
