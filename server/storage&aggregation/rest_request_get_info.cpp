#include "rest_request_get_info.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "rest_json_cookbook.h"

using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetInfo::GetInfo()
{

}

GetInfo::~GetInfo()
{

}

void GetInfo::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{  
  ostream& stream_response = response.send();
  stream_response << json::Cookbook::PrepareServerInfo();
}
// ~~ stormy::rest::request::GetInfo
}}}