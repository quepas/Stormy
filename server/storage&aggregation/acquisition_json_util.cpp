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

entity::Station ExtractStation(string json_response)
{
	entity::Station station;
	Reader reader;
	Value root;
	reader.parse(json_response, root, false);	
	if(!root["station"].isNull()) 
	{
		Value entry = root["station"];    	
		station.uid = entry["uid"].asString();
		station.name = entry["name"].asString();
		station.url = entry["url"].asString();
		station.refresh_time = entry["refresh_time"].asInt();
	}
	return station;
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

entity::Metrics ExtractMetrics(string json_response)
{
	entity::Metrics metrics;
	Reader reader;
	Value root;
	reader.parse(json_response, root, false);
	if(!root["metrics"].isNull()) 
	{
		Value entry = root["metrics"];	
		metrics.code = entry["code"].asString();
		metrics.equivalents = entry["equivalents"].asString();	
		metrics.type = entry["type"].asString();
		metrics.unit = entry["unit"].asString();
		metrics.format = entry["format"].asString();
	}
	return metrics;
}

vector<string> ExtractSimpleListElements(string json_response, string list_key)
{
  vector<string> list_elements;
  Reader reader;
  Value root;
  reader.parse(json_response, root, false);

  if (root[list_key].isArray()) {
    Value metrics = root[list_key];
    for (auto it = metrics.begin(); it != metrics.end(); ++it) {      
      list_elements.push_back((*it).asString());
    }
  }
  return list_elements;
}

// ~~ stormy::acquisition::json::util
}}}
