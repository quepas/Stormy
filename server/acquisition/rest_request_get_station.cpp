#include "rest_request_get_station.h"

#include "MongoDBHandler.h"
#include "../../common/util.h"
#include "rest_json_cookbook.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetStation::GetStation()
{

}

GetStation::~GetStation()
{

}

void GetStation::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
	ostream& ostr = response.send();
	auto stations = Stormy::MongoDBHandler::get().getStationsData();
	ostr << json::cookbook::PrepareStations(stations);
}
// ~~ stormy::rest::request::GetStation
}}}
