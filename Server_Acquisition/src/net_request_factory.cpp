#include "net_request_factory.hpp"
#include "net_get_requests.hpp"
#include "common/net_common_requests.hpp"
#include "common/rest_constant.h"
#include "common/util.h"

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>

using stormy::common::IsMatch;
using namespace stormy::common::rest;
using Poco::Logger;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;
using std::string;

namespace stormy {
  namespace net {

HTTPRequestHandler* AcquisitionRequestFactoryAction::createRequestHandler(const HTTPServerRequest& request, DatabaseContext context, Logger& logger_)
{
  string URI = request.getURI();
  logger_.information("[rest/Factory] Handling request: " + URI);

  if (request.getMethod() == HTTPRequest::HTTP_GET) {
    if (IsMatch(URI, constant::station_request_pattern) ||
      IsMatch(URI, constant::station_info_request_pattern)) {
      return new GetStation(URI, { context.db_handler });
    }
    else if (IsMatch(URI, constant::meteo_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern +
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_ts_request_pattern)) {
      return new GetMeteo(URI, { context.db_handler });
    }
    else if (IsMatch(URI, constant::metrics_request_pattern) ||
      IsMatch(URI, constant::metrics_info_request_pattern)) {
      return new GetMetrics(URI, { context.db_handler });
    }
    else if (IsMatch(URI, constant::info_request_pattern)) {
      return new GetInfo(URI, { context.db_handler });
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
// ~~ stormy::net::RequestFactory
