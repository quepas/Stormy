#include "net_get_requests.hpp"

#include "common/entity_station.h"
#include "common/rest_cookbook.h"
#include "common/rest_constant.h"
#include "common/util.h"

#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/NumberParser.h>
#include <vector>

using namespace stormy::common::rest;
using boost::copy;
using boost::adaptors::map_keys;
using Poco::NumberParser;
using Poco::UInt64;
using std::string;
using std::time_t;
using std::vector;

namespace stormy {
  namespace net {

string GetStationAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  auto path_segments = parsed_uri.getPathSegments();
  // api: /station
  if (path_segments.size() == 1) {
    auto stations = db_context.db_handler.GetStations();
    return cookbook::PrepareStationUIDs(stations);
  }
  // api: /station/:station_uid
  else if (path_segments.size() == 2) {
    auto station = db_context.db_handler.GetStationByUID(path_segments[1]);
    if (!station.id.empty()) {
      return cookbook::PrepareStationInfo(station);
    }
    else {
      return cookbook::PrepareError(
        "Bad station UID.", "Use proper station UID.");
    }
  }
}

string GetMeteoAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  auto& database_handler = db::MongoHandler::get();
  auto path_segments = parsed_uri.getPathSegments();
  auto query_segments = parsed_uri.getQuerySegments();

  // api: /meteo
  if (path_segments.size() == 1) {
    auto stations = database_handler.GetStations();
    std::vector<StationData> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      unsigned int count = database_handler
        .CountMeteo(it->id);
      if (count > 0)
        stations_with_any_meteo.push_back(*it);
    }
    return cookbook::PrepareStationUIDsWithAny(
      stations_with_any_meteo,
      constant::METEO);
  }
  // api: /meteo/:station_uid
  else if (path_segments.size() == 2) {
    if (query_segments.empty()) {
      auto measurements = database_handler
        .GetAllMeteo(path_segments[1]);
      vector<time_t> keys;
      copy(measurements | map_keys, back_inserter(keys));
      return cookbook::PrepareMeteoTimestamps(keys);
    }
    else {
      auto from_index = query_segments.find(constant::from_parameter);
      auto to_index = query_segments.find(constant::to_parameter);

      time_t from_ts = 0, to_ts = common::LocaltimeNow();
      UInt64 temporary_ts;
      if (from_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          from_index->second, temporary_ts)) {
          from_ts = temporary_ts;
        }
      }
      if (to_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          to_index->second, temporary_ts)) {
          to_ts = temporary_ts;
        }
      }
      auto measurements = database_handler
        .GetMeteoBetween(path_segments[1], from_ts, to_ts);
      return cookbook::PrepareMeteoSets(measurements);
    }
  }
  // api: /meteo/:station_uid/:timestamp
  else if (path_segments.size() == 3) {
    time_t timestamp = stol(path_segments[2]);

    auto measurements = database_handler
      .GetMeteo(path_segments[1], timestamp);
    return cookbook::PrepareMeteoSets(measurements);
  }
}

string GetMetricsAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  auto path_segments = parsed_uri.getPathSegments();
  auto metrics = db_context.db_handler.GetMeteoElements();

  if (path_segments.size() == 1) {
    return cookbook::PrepareMetricsCodes(metrics);
  }
  else if (path_segments.size() == 2) {
    for (auto it = metrics.begin(); it != metrics.end(); ++it) {
      if (it->id == path_segments[1]) {
        return cookbook::PrepareMetricsInfo(*it);
      }
    }
    return cookbook::PrepareError(
      "Bad request. Unknown metrics code.",
      "Use existing metrics code.");
  }
  else {
    return cookbook::PrepareError(
      "Bad request. Too much URI segments",
      "Create proper metrics request.");
  }
}

string GetInfoAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  return cookbook::PrepareServerInfo("Acquisition Server #1", "A", "UTC/GMT +1");
}

}}
// ~~ stormy::net::
