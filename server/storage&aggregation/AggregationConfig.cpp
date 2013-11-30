#include "AggregationConfig.h"

using namespace Stormy;
using namespace std;

AggregationConfig::AggregationConfig( string path )
	:	YAMLConfig(path),
		configuration()
{
	mapIntoConfiguration();
}

AggregationConfig::~AggregationConfig()
{

}

void AggregationConfig::mapIntoConfiguration()
{
	for(uint16 i = 0; i < Size(); ++i) {
		AggregationSetting setting;

		if(hasField("name", i)) {
			setting.name = asString("name", i);
		} else {
			cout << "No aggregation name. Try next..." << endl;
			continue;
		}
		if(hasField("interval", i)) {
			setting.interval = asString("interval", i);
		} else {
			cout << "No aggregation interval. Try next..." << endl;
			continue;
		}
		if(hasField("taskRefresh", i)) {
			setting.taskRefresh = asInt("taskRefresh", i);
		} else {
			cout << "No aggregation task refresh time. Try next..." 
				<< endl;
			continue;
		}
		if(hasField("turnOn", i)) {
			setting.turnOn = asBool("turnOn", i);
		} else {
			setting.turnOn = false;
		}
		configuration.push_back(setting);
	}
}
