#include "rest_request_get_meteo.h"

#include "../../common/rest_constant.h"
#include "../../common/rest_cookbook.h"
#include "../../common/entity_station.h"

#include <map>
#include <vector>
#include <Poco/NumberParser.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

using namespace stormy::common;
using namespace stormy::common::rest;

using std::ostream;
using std::string;
using std::map;
using std::vector;
using Poco::NumberParser;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;

namespace stormy {
  namespace rest {
    namespace request {

GetMeteo::GetMeteo(string uri, db::Storage* storage_database) 
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
  //auto& database_handler = db::MongoHandler::get();
  auto path_segments = uri_parser_.getPathSegments();
  auto query_segments = uri_parser_.getQuerySegments();
  ostream& ostr = response.send();

  // api: /meteo
  if (path_segments.size() == 1) {
    auto stations = storage_database_->GetStations();
    vector<entity::Station> stations_with_any_meteo;

    for (auto it = stations.begin(); it != stations.end(); ++it) {
      uint32_t count = storage_database_->CountStationMeasurements(it->uid);
      if (count > 0) 
        stations_with_any_meteo.push_back(*it);
    }
    ostr << cookbook::PrepareStationUIDsWithAnyMeteo(stations_with_any_meteo);
  }

  /*
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
  }*/
}
// ~~ stormy::test::request::GetMeteo
}}}