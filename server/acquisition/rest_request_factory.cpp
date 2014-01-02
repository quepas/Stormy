#include "rest_request_factory.h"

#include "rest_request_get_station.h"
#include "rest_request_get_metrics.h"
#include "rest_request_get_meteo.h"
#include "rest_request_get_info.h"
#include "../../common/util.h"
#include "../../common/rest_constant.h"
#include "../../common/rest_request_bad.h"

using namespace stormy::common;
using namespace stormy::common::rest;

using std::string;
using Poco::Logger;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace stormy {
  namespace rest {
    namespace request {

Factory::Factory()
  : logger_(Logger::get("rest/Factory"))
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

  if(IsMatch(URI, constant::station_request_pattern) ||
      IsMatch(URI, constant::station_info_request_pattern)) {
    return new GetStation(URI);
  }
  else if(IsMatch(URI, constant::meteo_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern +
                   constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_ts_request_pattern)) {
    return new GetMeteo(URI);
  }
  else if(IsMatch(URI, constant::metrics_request_pattern) ||
      IsMatch(URI, constant::metrics_info_request_pattern)) {
    return new GetMetrics(URI);
  }
  else if(IsMatch(URI, constant::info_request_pattern)) {
    return new GetInfo();
  } else {
    return new common::rest::request::Bad(URI);
  }
}
// ~~ stormy::rest::request::Factory
}}}
