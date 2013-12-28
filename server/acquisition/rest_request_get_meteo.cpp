#include "rest_request_get_meteo.h"

#include "MongoDBHandler.h"
#include "../../common/util.h"
#include "../../common/rest_cookbook.h"
#include "rest_json_cookbook.h"
#include "rest_constant.h"

#include <ctime>
#include <cstdint>
#include <map>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common::rest;
using std::map;
using std::make_pair;
using std::string;
using std::stol;
using std::ostream;
using std::time_t;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

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
    auto measurements = database_handler.getMeteoData(path_segments[1]);
    ostr << cookbook::PrepareMeteoTimestamps(measurements);
  }
  // api: /meteo/:station_uid/:timestamp
  else if (path_segments.size() == 3) {
    time_t timestamp = stol(path_segments[2]);

    auto measurements = database_handler.GetMeasureSetsForStationBetweenTS(path_segments[1], timestamp, timestamp);
    ostr << cookbook::PrepareMeteoSets(measurements);
  }
}
// ~~ stormy::rest::request::GetMeteo
}}}
