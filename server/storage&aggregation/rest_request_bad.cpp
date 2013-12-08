#include "rest_request_bad.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "rest_json_cookbook.h"

using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

Bad::Bad(string URI)
  : URI_(URI)
{

}

Bad::~Bad()
{

}

void Bad::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response)
{
  ostream& stream_response = response.send();
  stream_response << json::Cookbook::prepareBadResponse(URI_);
}
// ~~ stormy::rest::request::Bad
}}}
