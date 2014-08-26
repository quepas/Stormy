#include "rest_request_get_station.h"

#include "db_storage.h"
#include "rest_cookbook.h"

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

GetStation::GetStation(string uri, DatabaseSetting storage_setting)
  : storage_setting_(storage_setting),
    uri_parser_(uri)
{

}

GetStation::~GetStation()
{

}

void GetStation::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{ 
  db::Storage database_storage_(storage_setting_);
  auto path_segments = uri_parser_.getPathSegments();
  ostream& ostr = response.send();

  // api: /station
  if (path_segments.size() == 1) {
    auto stations = database_storage_.GetStations();
    ostr << cookbook::PrepareStationUIDs(stations);
  }
  // api: /station/:station_uid
  else if (path_segments.size() == 2) {
    auto station = database_storage_.GetStationByUID(path_segments[1]);

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
