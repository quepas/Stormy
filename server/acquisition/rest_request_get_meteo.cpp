#include "rest_request_get_meteo.h"

#include "MongoDBHandler.h"
#include "../../common/util.h"
#include "../../common/rest_cookbook.h"
#include "../../common/rest_constant.h"
#include "rest_json_cookbook.h"

#include <ctime>
#include <cstdint>
#include <map>
#include <vector>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/NumberParser.h>

using namespace stormy::common::rest;
using boost::copy;
using boost::adaptors::map_keys;
using std::back_inserter;
using std::map;
using std::make_pair;
using std::string;
using std::stol;
using std::ostream;
using std::time_t;
using std::time;
using std::vector;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::NumberParser;
using Poco::UInt64;

namespace stormy {
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(string uri)
	:	uri_parser_(uri)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{	
	auto& database_handler = Stormy::MongoDBHandler::get();
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  ostream& ostr = response.send();

  // api: /meteo
  if (path_segments.size() == 1) {
    auto stations = database_handler.getStationsData();
    auto uid_size_map = map<string, uint32_t>();

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint32_t count = database_handler
        .CountMeasureSetsForStationByUID(it->uid);
      if (count > 0) 
        uid_size_map.insert(make_pair(it->uid, count));      
    }
    ostr << cookbook::PrepareMeteoCountPerStation(uid_size_map);
  }
  // api: /meteo/:station_uid
  else if (path_segments.size() == 2) {
    if (query_segments.empty()) {
      auto measurements = database_handler
        .GetAllMeasureSetsForStation(path_segments[1]);
      vector<time_t> keys;
      copy(measurements | map_keys, back_inserter(keys));      
      ostr << cookbook::PrepareMeteoTimestamps(keys);
    } else {
      auto from_index = query_segments.find(constant::from_parameter);
      auto to_index = query_segments.find(constant::to_parameter);

      time_t from_ts = 0, to_ts = LocaltimeNow();
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
        .GetMeasureSetsForStationBetweenTS(path_segments[1], from_ts, to_ts);
      ostr << cookbook::PrepareMeteoSets(measurements);
    }
  }
  // api: /meteo/:station_uid/:timestamp
  else if (path_segments.size() == 3) {
    time_t timestamp = stol(path_segments[2]);

    auto measurements = database_handler
      .GetMeasureSetsForStationAndTS(path_segments[1], timestamp);
    ostr << cookbook::PrepareMeteoSets(measurements);
  }
}
// ~~ stormy::rest::request::GetMeteo
}}}
