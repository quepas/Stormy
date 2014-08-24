#include "acquisition_config_station.h"

#include <yaml-cpp/yaml.h>
#include "yaml_util.h"
#include "util.h"

using namespace stormy::common;
using std::string;
using YAML::Node;
using YAML::LoadFile;

namespace stormy {
  namespace acquisition {
    namespace config {

Station::Station(string filepath)
  : configuration_()
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
    if(!yaml::IsDefined(it, "url"))
    {
      //std::cout << "Field url is not defined" << std::endl;
      return false;
    }
    if(!yaml::IsDefined(it, "read_script"))
    {
      //std::cout << "Field parserClass is not defined" << std::endl;
      return false;
    }
    if(!yaml::IsDefined(it, "update_time"))
    {
      //std::cout << "Field refreshTime is not defined" << std::endl;
      return false;
    }

    entity::Station station;
    station.url = yaml::GetString(it, "url");
    station.uid = MD5(station.url);
    station.parser_class = yaml::GetString(it, "read_script");
    station.refresh_time = yaml::GetLongNumber(it, "update_time");

    if(yaml::IsDefined(it, "name"))
      station.name = yaml::GetString(it, "name");

    configuration_.push_back(station);
  }
  return true;
}
// ~~ stormy::acquisition::config::Station
}}}
