#include "rest_json_cookbook.h"

#include "../../common/rest_constant.h"

// TODO: use something lightest for creating json
#include <mongo/client/dbclient.h>
#include <Poco/NumberFormatter.h>

using namespace stormy::common::rest;
using stormy::common::entity::Station;
using stormy::common::entity::Metrics;
using stormy::aggregation::entity::Period;
using std::string;
using std::map;
using std::vector;
using mongo::BSONObjBuilder;
using Poco::NumberFormatter;

namespace stormy {
  namespace rest {
    namespace json {

string Cookbook::PrepareServerInfo(const vector<Metrics>& metrics, 
  const std::vector<aggregation::entity::Period>& periods)
{
  string content = "{" + WrapAsJSONString(constant::json_server_marker) + ":{";
  content += WrapAsJSONString(constant::json_type) + ":";
  content += WrapAsJSONString(constant::json_storage_and_aggregation_server_type) + "},";
  content += WrapAsJSONString(constant::json_available_metrics_marker) + ":";
  content += PrepareMetricsSequence(metrics) + ",";  
  content += WrapAsJSONString(constant::json_available_periods_marker) + ":";
  content += PreparePeriodSequence(periods) + "}";
  return content;
}

string Cookbook::PrepareBadResponse(const string& uri)
{
  BSONObjBuilder bson_builder;
  // TODO: make this more useful
  bson_builder.append(WrapAsJSONString("type"), "bad request");
  bson_builder.append(WrapAsJSONString("reason"), "unknown uri");
  bson_builder.append(WrapAsJSONString("uri"), uri);
  return bson_builder.obj().toString();
}

string Cookbook::WrapAsJSONString(const string& text)
{
  return "\"" + text + "\"";
}

string Cookbook::PrepareStation(const Station& station)
{
  BSONObjBuilder bson_builder;
  bson_builder.append(WrapAsJSONString(constant::json_uid), station.uid);
  bson_builder.append(WrapAsJSONString(constant::json_name), station.name);
  bson_builder.append(WrapAsJSONString(constant::json_url), station.url);
  bson_builder.append(
    WrapAsJSONString(constant::json_refresh_time), station.refresh_time);
  return bson_builder.obj().toString();
}

string Cookbook::PrepareStations(const vector<Station>& stations)
{
  string content = "{\"" + constant::json_stations_data_marker + "\":[";
  for (auto it = stations.begin(); it != stations.end(); ++it) {
    content += PrepareStation(*it) + ",";
  } 
  if(stations.size() > 0)
    content.pop_back();	// remove unnecessary comma
  content += "]}";
  return content;
}

string Cookbook::PrepareEmpty()
{
  return constant::json_empty;
}

string Cookbook::PrepareTypedMeasurement(const map<time_t, string>& measurements)
{
  string content;
  string time_content;
  string data_content;

  for (auto it = measurements.begin(); it != measurements.end(); ++it) {
    time_content += (NumberFormatter::format(it->first) + ",");
    data_content += (it->second + ",");
  }
  // remove unnecessary comma
  if(measurements.size() > 0) {
    time_content.pop_back();
    data_content.pop_back();
  }    
  content.append("{");
  content.append(WrapAsJSONString(constant::json_measurement_size_marker));
  content.append(":" + NumberFormatter::format(measurements.size()) + ",");
  content.append(WrapAsJSONString(constant::json_measurement_time_marker));
  content.append(":[" + time_content + "],");
  content.append(WrapAsJSONString(constant::json_measurement_data_marker));
  content.append(":[" + data_content + "]}");
  return content;  
}

string Cookbook::PrepareMetricsSequence(const vector<Metrics>& metrics)
{
  string sequence = "[";
  for (auto it = metrics.begin(); it != metrics.end(); ++it) {   
    sequence.append(WrapAsJSONString(it->code) + ",");
  }  
  if(metrics.size() > 0)
    sequence.pop_back();  // remove unnecessary comma
  sequence += "]";  
  return sequence;
}

string Cookbook::PreparePeriodSequence(const vector<Period>& periods)
{
  string sequence = "[";
  for (auto it = periods.begin(); it != periods.end(); ++it) {
    string element = "{";
    element += WrapAsJSONString(constant::json_name) + ":";
    element += WrapAsJSONString(it->name) + ",";
    element += WrapAsJSONString(constant::json_seconds) + ":";
    element += NumberFormatter::format(it->seconds);
    element += "}";
    sequence += element + ",";    
  }
  if (periods.size() > 0) 
    sequence.pop_back();  // remove unnecessary comma
  sequence += "]";
  return sequence;
}
// ~~ stormy::rest::json::Mapper
}}}