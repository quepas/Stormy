#include "net_request_factory.hpp"

#include "common/net_common_requests.hpp"
#include "common/rest_constant.h"
#include "common/util.h"

using namespace stormy::common;
using namespace stormy::common::rest;
using std::string;
using Poco::Logger;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace stormy {
  namespace net {

HTTPRequestHandler* StorageRequestFactoryAction::createRequestHandler(const HTTPServerRequest& request, DatabaseContext context, Logger& logger_)
{
  auto db_storage = context.db_storage;
  auto db_aggregation = context.db_aggregate;
  string URI = request.getURI();
  logger_.information("[rest/Factory] Handling request: " + URI);

  if (request.getMethod() == HTTPRequest::HTTP_GET) {
    if (IsMatch(URI, constant::station_request_pattern) ||
      IsMatch(URI, constant::station_info_request_pattern)) {
      return new net::GetStation(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::aggregate_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_uid_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_request_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_request_pattern +
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::aggregate_station_period_time_request_pattern)) {
      return new net::GetAggregate(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::meteo_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern +
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_ts_request_pattern)) {
      return new net::GetMeteo(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::info_request_pattern)) {
      return new net::GetInfo(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::meteo_element_request_pattern) ||
      IsMatch(URI, constant::meteo_element_info_request_pattern)) {
      return new net::GetMetrics(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::export_request_pattern) ||
      IsMatch(URI, constant::export_station_uid_request_pattern) ||
      IsMatch(URI, constant::export_station_uid_request_pattern +
      constant::uri_query_vars_pattern)) {
      return new net::GetExport(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::operation_request_pattern)) {
      return new net::GetOperation(URI, { db_storage, db_aggregation });
    }
    else if (IsMatch(URI, constant::period_request_pattern)) {
      return new net::GetPeriod(URI, { db_storage, db_aggregation });
    }
    else {
      return net::CreateRequestBad_(URI);
    }
  }
  else {
    return net::CreateRequestBad_(URI);
  }
}

}}
// ~~ stormy::net::StorageRequestFactory
