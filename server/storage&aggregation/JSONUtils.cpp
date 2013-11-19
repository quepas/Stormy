#include "JSONUtils.h"

#include <iostream>
#include <json/reader.h>
#include <Poco/Timestamp.h>
#include <Poco/NumberParser.h>
#include "../../common/Utils.h"

using namespace Stormy;
using namespace Stormy::Data;
using namespace Poco;
using namespace std;
using namespace Json;

vector<shared_ptr<Station>> JSONUtils::extractStationsFromJSON( string content )
{
	auto result = vector<shared_ptr<Station>>();	
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["stations"].isArray()) 
	{
		Value stations = root["stations"];
		Utils::forEach(stations, [&](Value entry) {
			shared_ptr<Station> station(new Station());			
			station -> uid = entry["id"].asString();
			station -> name = entry["name"].asString();
			station -> url = entry["url"].asString();
			station -> refreshTime = entry["refreshTime"].asInt();
			result.push_back(station);
		});
	}	
	return result;
}

vector<shared_ptr<Measurement>> JSONUtils::extractMeasurementsFromJSON( string content )
{
	auto result = vector<shared_ptr<Measurement>>();
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["meteo"].isArray()) 
	{
		Value meteo = root["meteo"];		
		Utils::forEach(meteo, [&](Value stationEntry) {							
			auto members = stationEntry.getMemberNames();
			unsigned long timestamp = 
				NumberParser::parseUnsigned64(stationEntry["id"].asString());
			
			for(auto it = members.begin(); it != members.end(); ++it) {				
				std::string key = *it;				
				std::string value = stationEntry[key].asString();
			
				shared_ptr<Measurement> measurement(new Measurement());
				measurement -> timestamp = Timestamp(timestamp);
				measurement -> metrics = new Metrics(key);
				measurement -> value = value;								
				result.push_back(measurement);
			}			
		});		
	}
	return result;
}
