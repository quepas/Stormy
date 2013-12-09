#include "rest_request_factory.h"

#include "rest_request_get_station.h"
#include "rest_request_get_aggregate.h"
#include "rest_request_get_meteo.h"
#include "rest_request_get_info.h"
#include "rest_request_bad.h"

#include "../../common/Utils.h"
#include "../../common/rest_constant.h"

using Stormy::DBStorage;
using Stormy::DBAggregation;

using namespace stormy::common::rest;
using std::string;
using Poco::Logger;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace stormy {
  namespace rest {
    namespace request {

Factory::Factory(DBStorage* db_storage, DBAggregation* db_aggregation)
  : logger_(Logger::get("rest")),
    db_storage_(db_storage),
    db_aggregation_(db_aggregation_)
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

  if (Stormy::Utils::checkTextWithRegex(URI, constant::station_request_pattern)) {
    return new GetStation(db_storage_);
  } else if (Stormy::Utils::checkTextWithRegex(URI, constant::aggregate_request_pattern)) {
    return new GetAggregate(URI, db_aggregation_);
  } else if (Stormy::Utils::checkTextWithRegex(URI, constant::meteo_request_pattern + constant::uri_query_vars_pattern)) {
    return new GetMeteo(URI, db_storage_);
  } else if (Stormy::Utils::checkTextWithRegex(URI, constant::info_request_pattern)) {
    return new GetInfo();
  } else {
    return new Bad(URI);
  }
}
// ~~ stormy::rest::request::Factory
}}}