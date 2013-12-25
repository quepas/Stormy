#include "acquisition_config_station.h"

#include "../../common/yaml_util.h"
#include "../../common/util.h"

using std::string;
using YAML::Node;
using YAML::LoadFile;

namespace stormy {
  namespace acquisition {
    namespace config {

Station::Station(string filepath)
	:	configuration_()
{
	if(!load(filepath))
	{
		//std::cout << "Couldn't load file properly: " << filepath << std::endl;
		configuration_.clear();
	}
}

Station::~Station()
{
	configuration_.clear();
}

bool Station::load(string filepath)
{
	Node root = LoadFile(filepath);

	for(auto it = root.begin(); it != root.end(); ++it)
	{
		if(!common::yaml::IsDefined(it, "url"))
		{
			//std::cout << "Field url is not defined" << std::endl;
			return false;
		}
		if(!common::yaml::IsDefined(it, "parserClass"))
		{
			//std::cout << "Field parserClass is not defined" << std::endl;
			return false;
		}
		if(!common::yaml::IsDefined(it, "refreshTime"))
		{
			//std::cout << "Field refreshTime is not defined" << std::endl;
			return false;
		}

		Stormy::StationPtr meteoStation(new Stormy::Meteo::Station());
		meteoStation -> url = common::yaml::GetString(it, "url");
		meteoStation -> stationId = common::MD5(meteoStation -> url);
		meteoStation -> parserClass = common::yaml::GetString(it, "parserClass");
		meteoStation -> refreshTime = common::yaml::GetLongNumber(it, "refreshTime");

		if(common::yaml::IsDefined(it, "name"))
			meteoStation -> name = common::yaml::GetString(it, "name");

		configuration_.push_back(meteoStation);
	}
	return true;
}
// ~~ stormy::acquisition::config::Station
}}}
