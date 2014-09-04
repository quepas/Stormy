#include "acquisition_json_util.h"
#include "common/rest_constant.h"

#include <cassert>
#include <ctime>
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
  Reader reader;
  Value root;
  reader.parse(json_response, root, false);

  Value entry = root[rest::constant::STATION];
  assert(!entry.isNull());
  return entity::Station {
    entry[rest::constant::UID].asString(),
    entry[rest::constant::NAME].asString(),
    entry[rest::constant::URL].asString(),
    entry[rest::constant::UPDATE_TIME].asInt()
  };
}

map<time_t, vector<entity::Measurement>> ExtractMeasureSets(string json_response, string station_uid)
{
  map<time_t, vector<entity::Measurement>> result;
  Reader reader;
  Value root;
  reader.parse(json_response, root, false);

  if (root[rest::constant::METEO].isArray())
  {
    Value measure_sets = root[rest::constant::METEO];
    for (auto& measure_set : measure_sets) {
      vector<entity::Measurement> measure_vec;
      time_t measure_set_time;

      if (measure_set[rest::constant::TIMESTAMP].isNumeric() && !measure_set[rest::constant::DATA].isNull()) {
        measure_set_time = measure_set[rest::constant::TIMESTAMP].asUInt64();
        Value data_set = measure_set[rest::constant::DATA];
        auto data_members = data_set.getMemberNames();
        for (auto& data : data_members) {
          entity::Measurement measure;
          measure.code = data;
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
  Reader reader;
  Value root;
  reader.parse(json_response, root, false);

  Value entry = root[rest::constant::METRICS];
  assert(!entry.isNull());
  return entity::Metrics {
    entry[rest::constant::CODE].asString(),
    entry[rest::constant::LABELS].asString(),
    entry[rest::constant::TYPE].asString(),
    entry[rest::constant::UNIT].asString(),
    entry[rest::constant::FORMAT].asString() };
}

vector<string> ExtractSimpleListElements(string json_response, string list_key)
{
  Reader reader;
  Value root;
  reader.parse(json_response, root, false);

  vector<string> list_elements;
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
