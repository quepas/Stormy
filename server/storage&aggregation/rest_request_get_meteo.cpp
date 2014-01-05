#include "rest_request_get_meteo.h"

#include "db_storage.h"
#include "../../common/rest_constant.h"
#include "../../common/rest_cookbook.h"
#include "../../common/entity_station.h"

#include <map>
#include <vector>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/NumberParser.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common;
using namespace stormy::common::rest;

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
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(string uri, common::db::Setting storage_setting) 
  : uri_parser_(uri),
    storage_setting_(storage_setting)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response )
{ 
  db::Storage storage_database(storage_setting_);
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  ostream& ostr = response.send();

  // api: /meteo
  if (path_segments.size() == 1) {
    auto stations = storage_database.GetStations();
    vector<entity::Station> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint64_t count = storage_database.CountStationMeasurements(it->uid);
      if (count > 0) 
        stations_with_any_meteo.push_back(*it);
    }
    ostr << cookbook::PrepareStationUIDsWithAny(
              stations_with_any_meteo,
              constant::json_measurements);
  }
  // api: /meteo/:station_uid
  else if (path_segments.size() == 2) {
    if (query_segments.empty()) {
      auto measurements = storage_database
        .GetAllMeasureSetsForStation(path_segments[1]);
      vector<time_t> keys;
      copy(measurements | map_keys, back_inserter(keys));      
      ostr << cookbook::PrepareMeteoTimestamps(keys);
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
      auto measurements = storage_database
        .GetMeasureSetsForStationBetweenTS(path_segments[1], from_ts, to_ts);
      ostr << cookbook::PrepareMeteoSets(measurements);
    }
  }
  // api: /meteo/:station_uid/:timestamp
  else if (path_segments.size() == 3) {
    time_t timestamp = 
      common::MakeUTCIfPossible(stol(path_segments[2]));

    auto measurements = storage_database
      .GetMeasureSetsForStationAndTS(path_segments[1], timestamp);
    ostr << cookbook::PrepareMeteoSets(measurements);
  }
}
// ~~ stormy::test::request::GetMeteo
}}}