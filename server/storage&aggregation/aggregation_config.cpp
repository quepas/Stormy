#include "aggregation_config.h"

#include <cstdint>

using std::string;

namespace stormy {
  namespace aggregation {

Config::Config(string path)
	:	BaseConfig(path)		
{
	MapIntoConfiguration();
}

Config::~Config()
{

}

void Config::MapIntoConfiguration()
{
	for(uint16_t i = 0; i < Size(); ++i) {
		Setting setting;

		if(HasField("name", i)) {
			setting.name = AsString("name", i);
		} else {
			logger_.warning("No aggregation name. Try next...");
			continue;
		}
		if(HasField("interval", i)) {
			setting.interval = AsString("interval", i);
		} else {
			logger_.warning("No aggregation interval. Try next...");
			continue;
		}
		if(HasField("taskRefresh", i)) {
			setting.task_refresh = AsInt("taskRefresh", i);
		} else {
			logger_.warning("No aggregation task refresh time. Try next...");
			continue;
		}
		if(HasField("turnOn", i)) {
			setting.turn_on = AsBool("turnOn", i);
		} else {
			setting.turn_on = false;
      logger_.notice("Aggregation task " + setting.name + " is turn off.");
		}
		configuration_.push_back(setting);
	}
}
// ~~ stormy::aggregation::Config
}}
