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
      constant::json_station_uids + 
        WrapAsList(join(wrapped_station_uids, ",")) +
    "}";
}

string PrepareStationInfo(const entity::Station& station)
{
  return
    "{" +
      constant::json_uid + WrapAsString(station.uid) + "," +
      constant::json_name + WrapAsString(station.name) + "," +
      constant::json_parser_class + WrapAsString(station.parser_class) + "," +
      constant::json_refresh_time + WrapAsString(station.refresh_time) + "," +
      constant::json_url + WrapAsString(station.url) +
    "}";
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
  vector<string> measurement_ids;
  string content = constant::json_measurements;
  for (auto it = measurements.begin(); it != measurements.end(); ++it) {
    auto timestamp = it->timestamp;
    measurement_ids.push_back(to_string(mktime(&timestamp)));
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

// ~~ stormy::common::rest::cookbook
}}}}
