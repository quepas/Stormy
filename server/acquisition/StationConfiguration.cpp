#include "StationConfiguration.h"
#include "../../common/yaml_util.h"
#include "../../common/Utils.h"

using namespace Stormy::Meteo;
using namespace stormy::common;

StationConfiguration::StationConfiguration( std::string filePath )
	:	configuration()
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
		if(!yaml::Util::IsDefined(it, "url"))
		{
			std::cout << "Field url is not defined" << std::endl;
			return false;
		}
		if(!yaml::Util::IsDefined(it, "parserClass"))
		{
			std::cout << "Field parserClass is not defined" << std::endl;
			return false;
		}
		if(!yaml::Util::IsDefined(it, "refreshTime"))
		{
			std::cout << "Field refreshTime is not defined" << std::endl;
			return false;
		}

		StationPtr meteoStation(new Station());
		meteoStation -> url = yaml::Util::GetString(it, "url");
		meteoStation -> stationId = Utils::md5(meteoStation -> url);
		meteoStation -> parserClass = yaml::Util::GetString(it, "parserClass");
		meteoStation -> refreshTime = yaml::Util::GetLongNumber(it, "refreshTime");

		if(yaml::Util::IsDefined(it, "name"))
			meteoStation -> name = yaml::Util::GetString(it, "name");

		configuration.push_back(meteoStation);
	}
	return true;
}
