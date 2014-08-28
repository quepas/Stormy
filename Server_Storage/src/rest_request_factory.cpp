#include "rest_request_factory.h"

#include "net_get_requests.hpp"
#include "net_common_requests.hpp"

#include "util.h"
#include "rest_constant.h"

using namespace stormy::common;
using namespace stormy::common::rest;
using std::string;
using Poco::Logger;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace stormy {
  namespace rest {
    namespace request {

Factory::Factory(DatabaseSetting db_storage, DatabaseSetting db_aggregation)
  : logger_(Logger::get("rest/Factory")),
    db_storage_(db_storage),
    db_aggregation_(db_aggregation)
{

}

Factory::~Factory()
{

}

HTTPRequestHandler* Factory::createRequestHandler(
  const HTTPServerRequest& request)
{
  string URI = request.getURI();
  logger_.information("[rest/Factory] Handling request: " + URI);

  if (request.getMethod() == HTTPRequest::HTTP_GET) {
    if (IsMatch(URI, constant::station_request_pattern) ||
      IsMatch(URI, constant::station_info_request_pattern)) {
      return new net::GetStation(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::aggregate_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_uid_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_request_pattern +
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_time_request_pattern)) {
      return new net::GetAggregate(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::meteo_request_pattern) ||      
      IsMatch(URI, constant::meteo_station_uid_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern + 
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_ts_request_pattern)) {
      return new net::GetMeteo(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::info_request_pattern)) {
      return new net::GetInfo(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::metrics_request_pattern) ||
      IsMatch(URI, constant::metrics_info_request_pattern)) {
      return new net::GetMetrics(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::export_request_pattern) ||
      IsMatch(URI, constant::export_station_uid_request_pattern) ||
      IsMatch(URI, constant::export_station_uid_request_pattern +
      constant::uri_query_vars_pattern)) {
      return new net::GetExport(URI, { db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::operation_request_pattern)) {
      return new net::GetOperation(URI,{ db_storage_, db_aggregation_ });
    } else if (IsMatch(URI, constant::period_request_pattern)) {
      return new net::GetPeriod(URI, { db_storage_, db_aggregation_ });
    } else {
      return net::CreateRequestBad_(URI);
    }
  } else {
    return net::CreateRequestBad_(URI);
  }
}
// ~~ stormy::rest::request::Factory
}}}