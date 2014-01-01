#include "rest_request_get_aggregate.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::string;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetAggregate::GetAggregate(string uri, common::db::Setting aggregate_setting)
  : uri_parser_(uri),
    aggregate_setting_(aggregate_setting)
{

}

GetAggregate::~GetAggregate()
{

}

void GetAggregate::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response )
{

}
// ~~ stormy::rest::request::GetAggregate
}}}
