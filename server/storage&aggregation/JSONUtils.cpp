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

StationPtrVector JSONUtils::extractStationsFromJSON( string content )
{
	auto result = StationPtrVector();	
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["stations"].isArray()) 
	{
		Value stations = root["stations"];
		Utils::forEach(stations, [&](Value entry) {
			StationPtr station(new Station());			
			station -> uid = entry["id"].asString();
			station -> name = entry["name"].asString();
			station -> url = entry["url"].asString();
			station -> refresh_time = entry["refreshTime"].asInt();
			result.push_back(station);
		});
	}
	return result;
}

MeasurementPtrVector JSONUtils::extractMeasurementsFromJSON( string content )
{
	auto result = MeasurementPtrVector();
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
			
				MeasurementPtr measurement(new Measurement());
				measurement -> timestamp = Timestamp(timestamp);
				measurement -> metrics = new Metrics(key);
				measurement -> value = value;								
				result.push_back(measurement);
			}			
		});		
	}
	return result;
}

MetricsPtrVector JSONUtils::extractMetricsFromJSON( string content )
{
	auto result = MetricsPtrVector();

	Reader reader;
	Value root;
	reader.parse(content, root, false);

	if(root["metrics"].isArray()) 
	{
		Value metrics = root["metrics"];
		Utils::forEach(metrics, [&](Value entry) {
			MetricsPtr metric(new Metrics());
			metric -> code = entry["code"].asString();
			metric -> equivalents = entry["name"].asString();	
			metric -> value_type = entry["type"].asString();
			metric -> value_unit = entry["unit"].asString();
			metric -> value_format = entry["format"].asString();
			result.push_back(metric);
		});
	}
	return result;
}
