#include "JSONUtils.h"

#include <iostream>
#include <json/reader.h>
#include "../../common/Utils.h"

using namespace Stormy;
using namespace Stormy::Data;
using namespace std;
using namespace Json;

vector<Station*> JSONUtils::extractStationsFromJSON( string content )
{
	auto result = vector<Station*>();	
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["stations"].isArray()) 
	{
		Value stations = root["stations"];
		Utils::forEach(stations, [&](Value entry){
			Station* station = new Station();
			station -> id = entry["id"].asString();
			station -> name = entry["name"].asString();
			station -> url = entry["url"].asString();
			station -> refreshTime = entry["refreshTime"].asInt64();
			result.push_back(station);
		});
	}	
	return result;
}
