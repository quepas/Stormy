#include "rest_request_get_meteo.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(Stormy::DBStorage* storage_database)
  : storage_database_(storage_database)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response )
{

}
// ~~ stormy::test::request::GetMeteo
}}}