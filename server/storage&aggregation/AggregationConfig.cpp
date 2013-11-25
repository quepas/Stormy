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
		AggregationSettings settings;

		if(hasField("name", i)) {
			settings.name = asString("name", i);
		} else {
			cout << "No aggregation name. Try next..." << endl;
			continue;
		}
		if(hasField("interval", i)) {
			settings.interval = asString("interval", i);
		} else {
			cout << "No aggregation interval. Try next..." << endl;
			continue;
		}
		if(hasField("taskRefresh", i)) {
			settings.taskRefresh = asInt("taskRefresh", i);
		} else {
			cout << "No aggregation task refresh time. Try next..." 
				<< endl;
			continue;
		}
		configuration.push_back(settings);
	}
}
