#include "rest_request_get_aggregate.h"

#include "rest_json_cookbook.h"
#include "db_storage.h"
#include "db_aggregate.h"
#include "aggregation_entity_period.h"
#include "entity_station.h"
#include "rest_constant.h"
#include "rest_cookbook.h"

#include <vector>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/NumberParser.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using std::string;
using std::ostream;
using std::vector;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::UInt64;
using Poco::NumberParser;

namespace stormy {
  namespace rest {
    namespace request {

GetAggregate::GetAggregate(
  std::string uri, 
  DatabaseSetting storage_setting,
  DatabaseSetting aggregate_setting)
  : uri_parser_(uri),
    storage_setting_(storage_setting),
    aggregate_setting_(aggregate_setting)
{

}

GetAggregate::~GetAggregate()
{

}

void GetAggregate::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response )
{
  db::Storage storage_database(storage_setting_);
  db::Aggregate aggregate_database(aggregate_setting_);
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  ostream& ostr = response.send();

  // api: /aggregate
  if (path_segments.size() == 1) {
    auto stations = storage_database.GetStations();
    vector<entity::Station> station_with_any_aggregate;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint64_t count = aggregate_database.CountStationAggregates(it->uid);      
      if (count > 0)
        station_with_any_aggregate.push_back(*it);
    }
    ostr << cookbook::PrepareStationUIDsWithAny(
              station_with_any_aggregate, 
              constant::json_aggregates);
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
    ostr << cookbook::PreparePeriodNamesWithAny(
              periods_with_any_aggregate, 
              constant::json_aggregates);
  }
  // api: /aggregate/:station_uid/:period_name
  else if (path_segments.size() == 3) {
    if (query_segments.empty()) {      
      auto start_times = aggregate_database
        .SelectAllDistinctAggregateTSForStationPeriod(
          path_segments[1], 
          path_segments[2]);
      ostr << cookbook::PrepareStationPeriodStartTimes(start_times);
    } else {
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
      ostr << json::Cookbook::PrepareAggregateSets(aggregates);
    }
  }
  // api: /aggregate/:station_uid/:period_name/:timestamp
  else if (path_segments.size() == 4) { 
    time_t timestamp = common::
      MakeUTCIfPossible(stol(path_segments[3]));

    auto aggregate_sets = aggregate_database
      .GetAggregateSetsForStationPeriodAndTS(
        path_segments[1], path_segments[2], timestamp);
    ostr << json::Cookbook::PrepareAggregateSets(aggregate_sets);
  }
}
// ~~ stormy::rest::request::GetAggregate
}}}
