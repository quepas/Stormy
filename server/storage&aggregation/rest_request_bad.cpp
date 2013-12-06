#include "rest_request_bad.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "rest_json_mapper.h"

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
  ostream& stream_resposne = response.send();
  stream_resposne << json::Mapper::prepareBadResponse(URI_);
}
// ~~ stormy::rest::request::Bad
}}}
