#include "net_get_requests.hpp"

#include "db_aggregate.h"
#include "db_storage.h"
#include "rest_constant.h"
#include "rest_cookbook.h"
#include "rest_csv_cookbook.h"
#include "rest_json_cookbook.h"
#include "entity_station.h"

#include <map>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/NumberParser.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using boost::split;
using boost::is_any_of;
using boost::copy;
using boost::adaptors::map_keys;
using std::back_inserter;
using std::ostream;
using std::string;
using std::map;
using std::vector;
using Poco::NumberParser;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::UInt64;

namespace stormy {
  namespace net {

string GetStationAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Storage database_storage_(db_context.db_storage);
  auto path_segments = parsed_uri.getPathSegments();

  // api: /station
  if (path_segments.size() == 1) {
    auto stations = database_storage_.GetStations();
    return cookbook::PrepareStationUIDs(stations);
  }
  // api: /station/:station_uid
  else if (path_segments.size() == 2) {
    auto station = database_storage_.GetStationByUID(path_segments[1]);

    if (!station.uid.empty()) {
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
  db::Storage storage_database(db_context.db_storage);
  auto path_segments = parsed_uri.getPathSegments();
  auto query_segments = parsed_uri.getQuerySegments();

  // api: /meteo
  if (path_segments.size() == 1) {
    auto stations = storage_database.GetStations();
    vector<entity::Station> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint64_t count = storage_database.CountStationMeasurements(it->uid);
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
      auto measurements = storage_database
        .GetAllMeasureSetsForStation(path_segments[1]);
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
          from_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      if (to_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          to_index->second, temporary_ts)) {
          to_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      auto measurements = storage_database
        .GetMeasureSetsForStationBetweenTS(path_segments[1], from_ts, to_ts);
      return cookbook::PrepareMeteoSets(measurements);
    }
  }
  // api: /meteo/:station_uid/:timestamp
  else if (path_segments.size() == 3) {
    time_t timestamp = common::
      MakeUTCIfPossible(stol(path_segments[2]));

    auto measurements = storage_database
      .GetMeasureSetsForStationAndTS(path_segments[1], timestamp);
    return cookbook::PrepareMeteoSets(measurements);
  }
}

string GetMetricsAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Storage database_storage_(db_context.db_storage);
  auto path_segments = parsed_uri.getPathSegments();
  auto metrics = database_storage_.GetMetrics();

  if (path_segments.size() == 1) {
    return cookbook::PrepareMetricsCodes(metrics);
  }
  else if (path_segments.size() == 2) {
    for (auto it = metrics.begin(); it != metrics.end(); ++it) {
      if (it->code == path_segments[1]) {
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

string GetAggregateAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Storage storage_database(db_context.db_storage);
  db::Aggregate aggregate_database(db_context.db_storage);
  auto path_segments = parsed_uri.getPathSegments();
  auto query_segments = parsed_uri.getQuerySegments();

  // api: /aggregate
  if (path_segments.size() == 1) {
    auto stations = storage_database.GetStations();
    vector<entity::Station> station_with_any_aggregate;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint64_t count = aggregate_database.CountStationAggregates(it->uid);
      if (count > 0)
        station_with_any_aggregate.push_back(*it);
    }
    return cookbook::PrepareStationUIDsWithAny(
      station_with_any_aggregate,
      constant::AGGREGATE);
  }
  // api: /aggregate/:station_uid
  else if (path_segments.size() == 2) {
    auto periods = aggregate_database.GetPeriods();
    vector<string> periods_with_any_aggregate;

    for (auto it = periods.begin(); it != periods.end(); ++it) {
      uint64_t count = aggregate_database
        .CountStationPeriodAggregates(path_segments[1], it->name);
      if (count > 0)
        periods_with_any_aggregate.push_back(it->name);
    }
    return cookbook::PreparePeriodNamesWithAny(
      periods_with_any_aggregate,
      constant::AGGREGATE);
  }
  // api: /aggregate/:station_uid/:period_name
  else if (path_segments.size() == 3) {
    if (query_segments.empty()) {
      auto start_times = aggregate_database
        .SelectAllDistinctAggregateTSForStationPeriod(
        path_segments[1],
        path_segments[2]);
      return cookbook::PrepareStationPeriodStartTimes(start_times);
    }
    else {
      auto from_index = query_segments.find(constant::from_parameter);
      auto to_index = query_segments.find(constant::to_parameter);

      time_t from_ts = 0, to_ts = common::LocaltimeNow();
      UInt64 temporary_ts;
      if (from_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          from_index->second, temporary_ts)) {
          from_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      if (to_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          to_index->second, temporary_ts)) {
          to_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      auto aggregates = aggregate_database
        .GetAggregateSetsForStationPeriodBetweenTS(
        path_segments[1],
        path_segments[2],
        from_ts,
        to_ts);
      return rest::json::Cookbook::PrepareAggregateSets(aggregates);
    }
  }
  // api: /aggregate/:station_uid/:period_name/:timestamp
  else if (path_segments.size() == 4) {
    time_t timestamp = common::
      MakeUTCIfPossible(stol(path_segments[3]));

    auto aggregate_sets = aggregate_database
      .GetAggregateSetsForStationPeriodAndTS(
      path_segments[1], path_segments[2], timestamp);
    return rest::json::Cookbook::PrepareAggregateSets(aggregate_sets);
  }
}

string GetOperationAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Aggregate aggregate_storage(db_context.db_aggregate);
  auto operations = aggregate_storage.GetOperations();
  return rest::json::Cookbook::PrepareOperations(operations);
}

string GetPeriodAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Aggregate aggregate_storage(db_context.db_aggregate);
  auto periods = aggregate_storage.GetPeriods();
  return rest::json::Cookbook::PreparePeriods(periods);
}

string GetExportAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  db::Storage database_storage_(db_context.db_storage);
  auto path_segments = parsed_uri.getPathSegments();
  auto query_segments = parsed_uri.getQuerySegments();

  // api: /export
  if (path_segments.size() == 1) {
    auto stations = database_storage_.GetStations();
    vector<entity::Station> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint64_t count = database_storage_.CountStationMeasurements(it->uid);
      if (count > 0)
        stations_with_any_meteo.push_back(*it);
    }
    return cookbook::PrepareStationUIDsWithAny(
      stations_with_any_meteo,
      constant::METEO);
  }
  // api: /export/:station_uid
  else if (path_segments.size() == 2) {
    if (query_segments.empty()) {
      auto measurements = database_storage_
        .GetAllMeasureSetsForStation(path_segments[1]);
      auto valid_metrics = database_storage_.GetMetricsCodes();
      return rest::csv::PrepareMeteo(measurements, valid_metrics);
    }
    else {
      auto from_index = query_segments.find(constant::from_parameter);
      auto to_index = query_segments.find(constant::to_parameter);
      auto metrics_index = query_segments.find(constant::metrics_parameter);

      time_t from_ts = 0, to_ts = common::LocaltimeNow();
      vector<string> metrics;
      UInt64 temporary_ts;
      if (from_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          from_index->second, temporary_ts)) {
          from_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      if (to_index != query_segments.end()) {
        if (NumberParser::tryParseUnsigned64(
          to_index->second, temporary_ts)) {
          to_ts = common::MakeUTCIfPossible(temporary_ts);
        }
      }
      if (metrics_index != query_segments.end()) {
        vector<string> splitted_metrics;
        split(splitted_metrics, metrics_index->second, is_any_of(","));
        auto valid_metrics = database_storage_.GetMetricsCodes();

        for (auto it = splitted_metrics.begin(); it != splitted_metrics.end(); ++it) {
          if (find(valid_metrics.begin(), valid_metrics.end(), *it) != valid_metrics.end() && !it->empty()) {
            metrics.push_back(*it);
          }
        }
      }
      auto measurements = database_storage_
        .GetMeasureSetsForStationBetweenTS(path_segments[1], from_ts, to_ts);
      return rest::csv::PrepareMeteo(measurements, metrics);
    }
  }
}

string GetInfoAction::PrepareResponse(URIParser parsed_uri, DatabaseContext db_context)
{
  return cookbook::PrepareServerInfo("Storage&Aggregation Server #1", "S&A", "UTC/GMT +1");
}

}}
// ~~ stormy::net::
