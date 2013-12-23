#include "rest_request_get_aggregate.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using std::string;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetAggregate::GetAggregate(string uri, db::Aggregate* aggregate_database)
  : uri_parser_(uri),
    aggregate_database_(aggregate_database)
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
