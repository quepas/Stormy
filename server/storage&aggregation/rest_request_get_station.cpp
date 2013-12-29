#include "rest_request_get_station.h"

#include "../../common/rest_cookbook.h"

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

GetStation::GetStation(string uri, db::Storage* storage_database)
  : storage_database_(storage_database),
    uri_parser_(uri)
{

}

GetStation::~GetStation()
{

}

void GetStation::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{ 
  auto path_segments = uri_parser_.getPathSegments();
  ostream& ostr = response.send();

  // api: /station
  if (path_segments.size() == 1) {
    auto stations = storage_database_->GetStations();
    ostr << cookbook::PrepareStationUIDs(stations);
  }
  // api: /station/:station_uid
  else if (path_segments.size() == 2) {
    auto station = storage_database_->GetStationByUID(path_segments[1]);

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
