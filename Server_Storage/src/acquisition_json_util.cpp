#include "acquisition_json_util.h"

#include <ctime>
#include <iostream>
#include <json/reader.h>
#include <Poco/Timestamp.h>
#include <Poco/NumberParser.h>

using namespace stormy::common;

using std::string;
using std::vector;
using std::map;
using std::make_pair;
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

map<time_t, vector<entity::Measurement>> 
  ExtractMeasureSets(string json_response, string station_uid)
{
	map<time_t, vector<entity::Measurement>> result;
	Reader reader;
	Value root;
	reader.parse(json_response, root, false);
	
	if (root["measurements"].isArray()) 
	{
		Value measure_sets = root["measurements"];		
		for (auto it = measure_sets.begin(); it != measure_sets.end(); ++it) {
      vector<entity::Measurement> measure_vec;
      Value measure_set = *it;
      time_t measure_set_time;

      if (measure_set["timestamp"].isNumeric() && !measure_set["data"].isNull()) {
        measure_set_time = measure_set["timestamp"].asUInt64();
        Value data_set = measure_set["data"];
        auto data_members = data_set.getMemberNames();
        
        for (auto m_it = data_members.begin(); m_it != data_members.end(); ++m_it) {
          entity::Measurement measure;
          measure.code = *m_it;
          measure.timestamp = *gmtime(&measure_set_time);
          measure.station_uid = station_uid;          
          
          if (data_set[measure.code].isNumeric()) {
            measure.value_number = data_set[measure.code].asFloat();
            measure.is_numeric = true;
          } else {
            measure.value_text = data_set[measure.code].asString();
            measure.is_numeric = false;
          }
          measure_vec.push_back(measure);
        }
        result.insert(make_pair(measure_set_time, measure_vec));
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
