#include "rest_request_get_aggregate.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetAggregate::GetAggregate(Stormy::DBAggregation* aggregate_database)
  : aggregate_database_(aggregate_database)
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
