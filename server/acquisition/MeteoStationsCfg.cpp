#include "MeteoStationsCfg.h"
#include "../../common/YAMLUtils.h"

using namespace Stormy;

MeteoStationsCfg::MeteoStationsCfg( std::string filePath )
	:	configuration(std::vector<MeteoStation*>())
{
	if(!load(filePath))
	{
		std::cout << "Couldn't load file properly: " << filePath << std::endl;
		configuration.clear();
	}
}

MeteoStationsCfg::~MeteoStationsCfg()
{
	configuration.clear();
}

bool MeteoStationsCfg::reload()
{
	return false;
}

bool MeteoStationsCfg::load(std::string filePath)
{
	YAML::Node root = YAML::LoadFile(filePath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{
		if(!YAMLUtils::isDefined(it, "url"))
		{
			std::cout << "Field url is not defined" << std::endl;
			return false;
		}
		if(!YAMLUtils::isDefined(it, "parserClass"))
		{
			std::cout << "Field parserClass is not defined" << std::endl;
			return false;
		}			
		if(!YAMLUtils::isDefined(it, "refreshTime"))
		{
			std::cout << "Field refreshTime is not defined" << std::endl;
			return false;
		}

		MeteoStation* meteoStation = new MeteoStation();
		meteoStation->url = YAMLUtils::getString(it, "url");
		meteoStation->parserClass = YAMLUtils::getString(it, "parserClass");
		meteoStation->refreshTime = YAMLUtils::getNumber(it, "refreshTime");

		if(YAMLUtils::isDefined(it, "id"))
			meteoStation -> id = YAMLUtils::getString(it, "id");
		if(YAMLUtils::isDefined(it, "name"))
			meteoStation -> name = YAMLUtils::getString(it, "name");

		configuration.push_back(meteoStation);
	}
	return true;
}
