#include "rest_cookbook.h"

#include "rest_constant.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <Poco/NumberFormatter.h>

using boost::algorithm::join;
using boost::lexical_cast;
using std::to_string;
using std::string;
using std::map;
using std::mktime;
using std::time_t;
using std::vector;
using Poco::NumberFormatter;

namespace stormy {
  namespace common {
    namespace rest {      
      namespace cookbook {

string PrepareError(
  string error /*= "Bad request"*/, 
  string msg   /*= ""*/ )
{
  return 
    "{" + 
      constant::json_error + WrapAsString(error) + "," + 
      constant::json_msg + WrapAsString(msg) + 
    "}";
}

template<typename T>
string WrapAsString(const T value)
{
  return "\"" + lexical_cast<string>(value) + "\"";
}

template<typename T>
string WrapAsList(const T values)
{
  return "[" + lexical_cast<string>(values) + "]";
}

string WrapAsJSON(string content)
{
  return "{" + content + "}";
}

string PrepareStationUIDs(const vector<entity::Station>& stations)
{
  vector<string> wrapped_station_uids;
  for (auto it = stations.begin(); it != stations.end(); ++it) {
    wrapped_station_uids.push_back(
      WrapAsString(it->uid));
  }
  return
    "{" + 
      constant::json_stations +
        WrapAsList(join(wrapped_station_uids, ",")) +
    "}";
}

string PrepareStationInfo(const entity::Station& station)
{
  string content = constant::json_station;
  string station_info =  
    constant::json_uid + WrapAsString(station.uid) + "," +
    constant::json_name + WrapAsString(station.name) + "," +
    constant::json_refresh_time + NumberFormatter::format(station.refresh_time) + "," +
    constant::json_url + WrapAsString(station.url);
  content += WrapAsJSON(station_info);
  return WrapAsJSON(content);
}

string PrepareStationUIDsWithAnyMeteo(const vector<entity::Station>& stations)
{
  vector<string> station_uids;
  string content = constant::json_measurements;
  for (auto it = stations.begin(); it != stations.end(); ++it) {
    station_uids.push_back(it->uid);
  }
  content += WrapAsList(join(station_uids, ","));
  return WrapAsJSON(content);
}

string PrepareMeteoCountPerStation(
  const map<string, uint32_t>& uid_count_map)
{
  vector<string> prepared_pairs;
  string content = constant::json_measurements;
  for (auto it = uid_count_map.begin(); it != uid_count_map.end(); ++it) {
    prepared_pairs.push_back(WrapAsJSON(      
      constant::json_station_uid + WrapAsString(it->first) + "," +
      constant::json_count + WrapAsString(it->second)
    ));
  }
  content += WrapAsList(join(prepared_pairs, ","));
  return WrapAsJSON(content);
}

string PrepareMeteoTimestamps(const vector<entity::Measurement>& measurements)
{
  vector<time_t> timestamps;
  for (auto it = measurements.begin(); it != measurements.end(); ++it) {
    auto timestamp = it->timestamp;
    timestamps.push_back(mktime(&timestamp));
  }
  return PrepareMeteoTimestamps(timestamps);
}

string PrepareMeteoTimestamps(const vector<time_t>& timestamps)
{
  vector<string> measurement_ids;
  string content = constant::json_measurements;
  for (auto it = timestamps.begin(); it != timestamps.end(); ++it) {
    auto timestamp = *it;
    measurement_ids.push_back(to_string(timestamp));
  }
  content += WrapAsList(join(measurement_ids, ","));
  return WrapAsJSON(content);
}

string PrepareMeteoSets(
  const map<time_t, vector<entity::Measurement>>& ts_measure_sets_map)
{
  vector<string> measurement_sets;
  string content = constant::json_measurements;
  for (auto it = ts_measure_sets_map.begin(); it != ts_measure_sets_map.end(); ++it) {
    measurement_sets.push_back(PrepareMeteoSet(it->first, it->second));
  }
  content += WrapAsList(join(measurement_sets, ","));
  return WrapAsJSON(content);
}

string PrepareMeteoSet(
  time_t ts, 
  const vector<entity::Measurement>& measure_set)
{
  vector<string> measure_pairs;
  string content = constant::json_timestamp + to_string(ts) + ",";
  for (auto it = measure_set.begin(); it != measure_set.end(); ++it) {
    string row = WrapAsString(it->code) + ":";
    if(it->is_numeric) {
      row += NumberFormatter::format(it->value_number);
    } else {
      row += WrapAsString(it->value_text);
    }
    measure_pairs.push_back(row);
  }
  content += constant::json_data + WrapAsJSON(join(measure_pairs, ","));
  return WrapAsJSON(content);
}

string PrepareMetricsCodes(const vector<entity::Metrics>& metrics_vec)
{
  vector<string> metrics_codes;
  string content = constant::json_metrics;
  for (auto it = metrics_vec.begin(); it != metrics_vec.end(); ++it) {
    if (it->is_meteo) {
      metrics_codes.push_back(WrapAsString(it->code));
    }
  }
  content += WrapAsList(join(metrics_codes, ","));
  return WrapAsJSON(content);
}

string PrepareMetricsInfo(const entity::Metrics& metrics)
{
  string content = constant::json_metrics;
  string metrics_info =
    constant::json_code + WrapAsString(metrics.code) + "," +
    constant::json_equivalents + WrapAsString(metrics.equivalents) + "," +
    constant::json_type + WrapAsString(metrics.type) + "," +
    constant::json_unit + WrapAsString(metrics.unit) + "," +
    constant::json_format + WrapAsString(metrics.format);
  content += WrapAsJSON(metrics_info);
  return WrapAsJSON(content);
}

string PrepareServerInfo(string name, string type, string timezone)
{
  string content = constant::json_server;
  time_t current_time = time(nullptr) + 3600;
  string str_time = asctime(gmtime(&current_time));
  str_time.pop_back();  // remove new line character
  string server_info =
    constant::json_name + WrapAsString(name) + "," +
    constant::json_type + WrapAsString(type) + "," +
    constant::json_timezone + WrapAsString(timezone) + "," +
    constant::json_time + WrapAsString(str_time);
  content += WrapAsJSON(server_info);
  return WrapAsJSON(content);
}
// ~~ stormy::common::rest::cookbook
}}}}
