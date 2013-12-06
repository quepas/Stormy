#include "rest_request_factory.h"

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

  if (Stormy::Utils::checkTextWithRegex(URI, constant::stationPattern)) {
    return new GetStation();
  } else if (Stormy::Utils::checkTextWithRegex(URI, constant::infoPattern)) {
    return new GetInfo();
  } else {
    return new Bad(URI);
  }
}
// ~~ stormy::rest::request::Factory
}}}