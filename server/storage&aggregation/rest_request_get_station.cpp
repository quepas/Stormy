#include "rest_request_get_station.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

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

void GetStation::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{

}
// ~~ stormy::rest::request::GetStation
}}}
