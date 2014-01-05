#include "rest_json_cookbook.h"

#include "../../common/rest_cookbook.h"
#include "../../common/rest_constant.h"

// TODO: use something lightest for creating json
#include <boost/algorithm/string.hpp>
#include <mongo/client/dbclient.h>
#include <Poco/NumberFormatter.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using boost::algorithm::join;
using std::to_string;
using std::string;
using std::map;
using std::make_pair;
using std::vector;
using mongo::BSONObjBuilder;
using Poco::NumberFormatter;

namespace stormy {
  namespace rest {
    namespace json {

string Cookbook::PrepareServerInfo(const vector<entity::Metrics>& metrics, 
  const std::vector<aggregation::entity::Period>& periods)
{
  string content = "{" + constant::json_server + "{";
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

string Cookbook::PrepareStation(const entity::Station& station)
{
  BSONObjBuilder bson_builder;
  bson_builder.append(WrapAsJSONString(constant::json_uid), station.uid);
  bson_builder.append(WrapAsJSONString(constant::json_name), station.name);
  bson_builder.append(WrapAsJSONString(constant::json_url), station.url);
  bson_builder.append(
    WrapAsJSONString(constant::json_refresh_time), station.refresh_time);
  return bson_builder.obj().toString();
}

string Cookbook::PrepareStations(const vector<entity::Station>& stations)
{
  string content = "{\"" + constant::json_stations + "\":[";
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
  content.append(constant::json_size);
  content.append(NumberFormatter::format(measurements.size()) + ",");
  content.append(constant::json_times);
  content.append("[" + time_content + "],");
  content.append(constant::json_measurements);
  content.append("[" + data_content + "]}");
  return content;  
}

string Cookbook::PrepareMetricsSequence(const vector<entity::Metrics>& metrics)
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

string Cookbook::PreparePeriodSequence(const vector<aggregation::entity::Period>& periods)
{
  string sequence = "[";
  for (auto it = periods.begin(); it != periods.end(); ++it) {
    string element = "{";
    element += constant::json_name;
    element += WrapAsJSONString(it->name) + ",";
    element += constant::json_seconds;
    element += NumberFormatter::format(it->seconds);
    element += "}";
    sequence += element + ",";    
  }
  if (periods.size() > 0) 
    sequence.pop_back();  // remove unnecessary comma
  sequence += "]";
  return sequence;
}

string Cookbook::PrepareAggregateSets(
  const map<time_t, vector<aggregation::entity::Aggregate>>& aggregate_sets)
{
  vector<string> aggregate_sets_str;
  string content = constant::json_aggregates;
  for (auto it = aggregate_sets.begin(); it != aggregate_sets.end(); ++it) {
    aggregate_sets_str.push_back(PrepareAggregateSet(it->first, it->second));
  }
  content += cookbook::WrapAsList(join(aggregate_sets_str, ","));
  return cookbook::WrapAsJSON(content);
}

string Cookbook::PrepareAggregateSet(
  time_t time,
  const vector<aggregation::entity::Aggregate>& aggregate_set)
{
  vector<string> aggregated_object;
  string content = constant::json_start_time + to_string(time) + ",";
  auto code_aggregate_set = 
    ConvertAggregateSetToCodeAggregateSet(aggregate_set);

  for (auto it = code_aggregate_set.begin(); it != code_aggregate_set.end(); ++it) {
    string row = cookbook::WrapAsString(it->first) + ":";
    string data;
    vector<string> row_values;
    auto aggregates = it->second;

    for (auto a_it = aggregates.begin(); a_it != aggregates.end(); ++a_it) {
      string row_value = cookbook::WrapAsString(a_it->operation_name) + ":";
      row_value += NumberFormatter::format(a_it->value);
      row_values.push_back(row_value);
    }
    data += join(row_values, ",");
    row += cookbook::WrapAsJSON(data);
    aggregated_object.push_back(row);
  }  
  content += constant::json_data + cookbook::WrapAsJSON(join(aggregated_object, ","));
  return cookbook::WrapAsJSON(content);
}

map<string, vector<aggregation::entity::Aggregate>> Cookbook::
  ConvertAggregateSetToCodeAggregateSet(
    const vector<aggregation::entity::Aggregate>& aggregate_set )
{
  map<string, vector<aggregation::entity::Aggregate>> code_aggregate_sets;
  for (auto it = aggregate_set.begin(); it != aggregate_set.end(); ++it) {
    code_aggregate_sets[it->metrics_code].push_back(*it);
  }
  return code_aggregate_sets;
}
// ~~ stormy::rest::json::Mapper
}}}