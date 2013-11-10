#include "StationConfiguration.h"
#include "../../common/YAMLUtils.h"
#include "../../common/Utils.h"

using namespace Stormy::Meteo;

StationConfiguration::StationConfiguration( std::string filePath )
	:	configuration(std::vector<Station*>())
{
	if(!load(filePath))
	{
		std::cout << "Couldn't load file properly: " << filePath << std::endl;
		configuration.clear();
	}
}

StationConfiguration::~StationConfiguration()
{
	configuration.clear();
}

bool StationConfiguration::load(std::string filePath)
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

		Station* meteoStation = new Station();
		meteoStation -> url = YAMLUtils::getString(it, "url");
		meteoStation -> stationId = Utils::md5(meteoStation -> url);
		meteoStation -> parserClass = YAMLUtils::getString(it, "parserClass");
		meteoStation -> refreshTime = YAMLUtils::getLongNumber(it, "refreshTime");

		if(YAMLUtils::isDefined(it, "name"))
			meteoStation -> name = YAMLUtils::getString(it, "name");

		configuration.push_back(meteoStation);
	}
	return true;
}
