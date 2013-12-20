#include "AggregationConfig.h"

using namespace Stormy;
using namespace std;

AggregationConfig::AggregationConfig( string path )
	:	Config(path),
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

		if(HasField("name", i)) {
			setting.name = AsString("name", i);
		} else {
			cout << "No aggregation name. Try next..." << endl;
			continue;
		}
		if(HasField("interval", i)) {
			setting.interval = AsString("interval", i);
		} else {
			cout << "No aggregation interval. Try next..." << endl;
			continue;
		}
		if(HasField("taskRefresh", i)) {
			setting.taskRefresh = AsInt("taskRefresh", i);
		} else {
			cout << "No aggregation task refresh time. Try next..." 
				<< endl;
			continue;
		}
		if(HasField("turnOn", i)) {
			setting.turnOn = AsBool("turnOn", i);
		} else {
			setting.turnOn = false;
		}
		configuration.push_back(setting);
	}
}
