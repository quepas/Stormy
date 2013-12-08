#include "rest_request_get_meteo.h"

#include "../../common/rest_constant.h"

#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common::rest;
using std::ostream;
using std::string;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(string uri, Stormy::DBStorage* storage_database ) 
  : uri_parser_(uri),
    storage_database_(storage_database)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(HTTPServerRequest& request, 
  HTTPServerResponse& response )
{ 
  auto path_segments = uri_parser_.getPathSegments();
  
  ostream& stream_response = response.send();  
  if(path_segments.size() > 1) {

  } else {
    stream_response << constant::json_empty;
  }

  auto param = uri_parser_.getQuerySegments();
  for (auto it = param.begin(); it != param.end(); ++it) {
    //std::cout << "\t" << it->first << " : " << it->second << std::endl;
  }

  
}
// ~~ stormy::test::request::GetMeteo
}}}