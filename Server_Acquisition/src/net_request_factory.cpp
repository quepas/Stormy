#include "net_request_factory.h"
#include "net_request.hpp"
#include "rest_request_bad.h"
#include "rest_constant.h"
#include "util.h"

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

RequestFactory::RequestFactory(db::MongoHandler& db_handler)
  : db_handler_(db_handler),
    logger_(Logger::get("main"))
{

}

RequestFactory::~RequestFactory()
{

}

HTTPRequestHandler* RequestFactory::createRequestHandler(const HTTPServerRequest& request)
{
  string URI = request.getURI();
  logger_.information("[rest/Factory] Handling request: " + URI);

  if (request.getMethod() == HTTPRequest::HTTP_GET) {
    if (IsMatch(URI, constant::station_request_pattern) ||
      IsMatch(URI, constant::station_info_request_pattern)) {
      return new GetStation(URI, db_handler_);
    }
    else if (IsMatch(URI, constant::meteo_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_request_pattern +
      constant::uri_query_vars_pattern) ||
      IsMatch(URI, constant::meteo_station_uid_ts_request_pattern)) {
      return new GetMeteo(URI, db_handler_);
    }
    else if (IsMatch(URI, constant::metrics_request_pattern) ||
      IsMatch(URI, constant::metrics_info_request_pattern)) {
      return new GetMetrics(URI, db_handler_);
    }
    else if (IsMatch(URI, constant::info_request_pattern)) {
      return new GetInfo(URI, db_handler_);
    }
    else {
      return new common::rest::request::Bad(URI);
    }
  }
  else {
    return new common::rest::request::Bad(URI);
  }
}

}}
// ~~ stormy::net::RequestFactory
