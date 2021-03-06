#include "common/rest_cookbook.h"
#include "common/util.h"
#include "common/rest_constant.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <json/writer.h>

using boost::algorithm::join;
using boost::lexical_cast;
using boost::split;
using boost::is_any_of;
using std::to_string;
using std::string;
using std::map;
using std::mktime;
using std::time_t;
using std::vector;

namespace stormy {
  namespace common {
    namespace rest {
      namespace cookbook {

string PrepareError(
  string error /*= "Bad request"*/, 
  string msg   /*= ""*/ )
{
  Json::Value root;
  root[constant::ERROR_] = error;
  root[constant::MSG] = msg;
  return ToStyledString(root);
}

string ToStyledString(const Json::Value& value)
{
  Json::StyledWriter writer;
  return writer.write(value);
}

string PrepareStationUIDs(const vector<StationData>& stations)
{
  Json::Value root;
  for (unsigned idx = 0; idx < stations.size(); ++idx) {
    root[constant::STATION][idx] = stations[idx].id;
  }
  return ToStyledString(root);
}

string PrepareStationInfo(const StationData& station)
{
  Json::Value root;
  root[constant::UID] = station.id;
  root[constant::NAME] = station.name;
  root[constant::UPDATE_TIME] = station.update_time;
  root[constant::URL] = station.url;
  root[constant::TIMEZONE] = station.time_zone;
  return ToStyledString(root);
}

std::string PrepareStationUIDsWithAny(const std::vector<StationData>& stations, std::string data_key)
{
  Json::Value root;
  for (unsigned idx = 0; idx < stations.size(); ++idx) {
    root[data_key][idx] = stations[idx].id;
  }
  return ToStyledString(root);
}

string PrepareMeteoTimestamps(const vector<time_t>& timestamps)
{
  Json::Value root;
  for (unsigned idx = 0; idx < timestamps.size(); ++idx) {
    root[constant::METEO][idx] = to_string(timestamps[idx]);
  }
  return ToStyledString(root);
}

string PrepareMeteoSets(
  const map<time_t, vector<entity::Measurement>>& measure_map)
{
  Json::Value root;
  unsigned idx = 0;
  for (auto it = measure_map.begin(); it != measure_map.end(); ++it) {
    Json::Value meteo;
    for (auto& entry : it->second) {
      if (entry.is_numeric)
        meteo[entry.code] = entry.value_number;
      else
        meteo[entry.code] = entry.value_text;
    }
    meteo[constant::TIMESTAMP] = to_string(it->first);
    root[constant::METEO][idx++] = meteo;
  }
  return ToStyledString(root);
}

string PrepareMeteoElementIds(const vector<MeteoElement>& metrics)
{
  Json::Value root;
  for (unsigned idx = 0; idx < metrics.size(); ++idx) {
    root[constant::METRICS][idx] = metrics[idx].id;
  }
  return ToStyledString(root);
}

string PrepareMeteoElementDetails(const MeteoElement& metrics)
{
  Json::Value root;
  root[constant::CODE] = metrics.id;
  root[constant::TYPE] = metrics.type;
  /*root[constant::UNIT] = metrics.unit;
  root[constant::FORMAT] = metrics.format;
  root[constant::IS_METEO] = metrics.is_meteo;*/

  for (unsigned idx = 0; idx < metrics.labels.size(); ++idx) {
    root[constant::LABELS][idx] = metrics.labels[idx];
  }
  return ToStyledString(root);
}

string PrepareServerInfo(string name, string type, string timezone)
{
  Json::Value root;
  root[constant::STATUS] = "OK"; // ? ;)
  root[constant::NAME] = name;
  root[constant::TYPE] = type;
  root[constant::TIMEZONE] = timezone;
  time_t current_time = LocaltimeNow();
  string str_time = asctime(gmtime(&current_time));
  str_time.pop_back();  // remove new line character
  root[constant::TIME_NOW] = str_time;
  return ToStyledString(root);
}

string PreparePeriodNamesWithAny(const vector<string>& period_names, string data_key )
{
  vector<string> periods_as_json_string;
  Json::Value root;
  for (unsigned idx = 0; idx < period_names.size(); ++idx) {
    root[data_key][idx] = period_names[idx];
  }
  return ToStyledString(root);
}

string PrepareStationPeriodStartTimes(
  const vector<tm>& start_times)
{
  Json::Value root;
  for (unsigned idx = 0; idx < start_times.size(); ++idx) {
    tm start_time = start_times[idx];
    time_t timestamp = mktime(&start_time) + 3600;  // as local time!
    root[constant::AGGREGATE] = to_string(timestamp);
  }
  return ToStyledString(root);
}

// ~~ stormy::common::rest::cookbook
}}}}
