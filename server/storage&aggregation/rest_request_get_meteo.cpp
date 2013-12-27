#include "rest_request_get_meteo.h"

#include "../../common/rest_constant.h"
#include "rest_json_cookbook.h"

#include <Poco/NumberParser.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common::rest;
using std::ostream;
using std::string;
using Poco::NumberParser;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(string uri, db::Storage* storage_database ) 
  : uri_parser_(uri),
    storage_database_(storage_database)
{

}

GetMeteo::~GetMeteo()
{

}

void GetMeteo::handleRequest(
  HTTPServerRequest& request, 
  HTTPServerResponse& response )
{ 
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  auto metrics_index = query_segments.find(constant::type_parameter);
  auto from_last_hours_index = 
    query_segments.find(constant::from_last_hours_parameter);
  
  ostream& stream_response = response.send();  
  if(path_segments.size() > 1 && metrics_index != query_segments.end()) {
    string station_id = path_segments[1];
    string metrics_code = metrics_index->second;
    int hours = constant::default_from_last_hours;

    if(from_last_hours_index != query_segments.end()) {
      if(!NumberParser::tryParse(from_last_hours_index->second, hours)) {
        hours = constant::default_from_last_hours;
      }
    }    
    stream_response << json::Cookbook::PrepareTypedMeasurement(
      storage_database_->GetMeasurementFromLast(
        station_id, metrics_code, hours));
  } else {
    stream_response << json::Cookbook::PrepareEmpty();
  }
}
// ~~ stormy::test::request::GetMeteo
}}}