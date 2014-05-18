#include "rest_request_get_station.h"

#include "../../common/util.h"
#include "../../common/rest_cookbook.h"
#include "db_mongo_handler.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common::rest;
using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetStation::GetStation(string uri)
  : uri_parser_(uri)
{

}

GetStation::~GetStation()
{

}

void GetStation::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
  auto& database_handler = db::MongoHandler::get();
  auto path_segments = uri_parser_.getPathSegments();
  ostream& ostr = response.send();
  
  // api: /station
  if (path_segments.size() == 1) {
     auto stations = database_handler.getStationsData();
     ostr << cookbook::PrepareStationUIDs(stations);
  }
  // api: /station/:station_uid
  else if (path_segments.size() == 2) {
    auto station = database_handler.GetStationByUID(path_segments[1]);

    if(!station.uid.empty()) {
      ostr << cookbook::PrepareStationInfo(station);
    } else {
      ostr << cookbook::PrepareError(
        "Bad station UID.", "Use proper station UID.");
    }
  }
}
// ~~ stormy::rest::request::GetStation
}}}
