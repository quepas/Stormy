#include "rest_request_get_station.h"

#include "rest_json_cookbook.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetStation::GetStation(Stormy::DBStorage* storage_database)
  : storage_database_(storage_database)
{

}

GetStation::~GetStation()
{

}

void GetStation::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{ 
  ostream& stream_response = response.send();
  stream_response << 
    json::Cookbook::prepareStations(storage_database_->GetStations());
}
// ~~ stormy::rest::request::GetStation
}}}
