#include "rest_request_factory.h"

#include "rest_request_get_info.h"
#include "rest_request_bad.h"
#include "../../common/Utils.h"
#include "../../common/common_rest_constant.h"

using namespace stormy::common::rest;
using std::string;
using Poco::Logger;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPServerRequest;

namespace stormy {
  namespace rest {
    namespace request {

Factory::Factory()
  : logger_(Logger::get("rest"))
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

  if (Stormy::Utils::checkTextWithRegex(URI, constant::infoPattern)) {
    return new GetInfo();
  } else {
    return new Bad(URI);
  }
}
// ~~ stormy::rest::request::Factory
}}}