#include "acquisition_json_util.h"

#include <ctime>
#include <iostream>
#include <json/reader.h>
#include <Poco/Timestamp.h>
#include <Poco/NumberParser.h>

using namespace stormy::common;

using std::string;
using std::vector;
using std::time_t;
using Json::Reader;
using Json::Value;
using Poco::NumberParser;
using Poco::Timestamp;

namespace stormy {
  namespace acquisition {
    namespace json {

vector<entity::Station> ExtractStations(string content)
{
	auto result = vector<entity::Station>();	
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["stations"].isArray()) 
	{
		Value stations = root["stations"];
    for (auto it = stations.begin(); it != stations.end(); ++it) {
      auto entry = *it;
      entity::Station station;			
			station.uid = entry["id"].asString();
			station.name = entry["name"].asString();
			station.url = entry["url"].asString();
			station.refresh_time = entry["refresh_time"].asInt();
			result.push_back(station);
		}
	}
	return result;
}

vector<entity::Measurement> ExtractMeasurements(string content)
{
	auto result = vector<entity::Measurement>();
	Reader reader;
	Value root;
	reader.parse(content, root, false);
	
	if(root["meteo"].isArray()) 
	{
		Value meteo = root["meteo"];		
		for (auto it = meteo.begin(); it != meteo.end(); ++it) {
      auto entry = *it;
			auto members = entry.getMemberNames();
			time_t timestamp = 
				NumberParser::parseUnsigned64(entry["id"].asString());
			
			for(auto member_it = members.begin();
            member_it != members.end(); ++member_it) {				
				std::string key = *member_it;				
				std::string value = entry[key].asString();
			
				entity::Measurement measurement;
				measurement.timestamp = *gmtime(&timestamp);
				measurement.code = key;
				measurement.value_text = value;								
				result.push_back(measurement);
			}			
		}		
	}
	return result;
}

vector<entity::Metrics> ExtractMetrics(string content)
{
	auto result = vector<entity::Metrics>();

	Reader reader;
	Value root;
	reader.parse(content, root, false);

	if(root["metrics"].isArray()) 
	{
		Value metrics = root["metrics"];
		for (auto it = metrics.begin(); it != metrics.end(); ++it) {
      auto entry = *it;
			entity::Metrics metric;
			metric.code = entry["code"].asString();
			metric.equivalents = entry["name"].asString();	
			metric.type = entry["type"].asString();
			metric.unit = entry["unit"].asString();
			metric.format = entry["format"].asString();
			result.push_back(metric);
    }
	}
	return result;
}
// ~~ stormy::acquisition::json::util
}}}
