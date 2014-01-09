#include "rest_request_get_options.h"

#include "rest_constant.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::string;
using std::ostream;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace common {
    namespace rest {
      namespace request {

GetOptions::GetOptions(string URI)
{

}

GetOptions::~GetOptions()
{

}

void GetOptions::handleRequest(
  HTTPServerRequest& request,
  HTTPServerResponse& response)
{
  ostream& ostr = response.send();
  ostr << constant::json_empty;
}

// ~~ stormy::common::rest::request::GetOptions
}}}}
