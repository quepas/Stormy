#include "acquisition_config_station.h"

#include <yaml-cpp/yaml.h>
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

		common::entity::Station station;
		station.url = common::yaml::GetString(it, "url");
		station.uid = common::MD5(station.url);
		station.parser_class = common::yaml::GetString(it, "parserClass");
		station.refresh_time = common::yaml::GetLongNumber(it, "refreshTime");

		if(common::yaml::IsDefined(it, "name"))
			station.name = common::yaml::GetString(it, "name");

		configuration_.push_back(station);
	}
	return true;
}
// ~~ stormy::acquisition::config::Station
}}}
