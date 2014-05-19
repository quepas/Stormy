#include "rest_request_get_info.h"

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

GetInfo::GetInfo()
{

}

GetInfo::~GetInfo()
{

}

void GetInfo::handleRequest(
  HTTPServerRequest& request,
  HTTPServerResponse& response)
{
  ostream& ostr = response.send();
  ostr << cookbook::
    PrepareServerInfo("Acquisition Server #1", "A", "UTC/GMT +1");
}
// ~~ stormy::rest::request::GetInfo
}}}
