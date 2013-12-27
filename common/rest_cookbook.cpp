#include "rest_cookbook.h"

#include "rest_constant.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>

using boost::algorithm::join;
using boost::lexical_cast;
using std::string;
using std::vector;

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
// ~~ stormy::common::rest::cookbook
}}}}
