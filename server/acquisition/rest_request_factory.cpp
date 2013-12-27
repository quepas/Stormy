#include "rest_request_factory.h"

#include "rest_request_get_station.h"
#include "rest_request_get_metrics.h"
#include "rest_request_get_meteo.h"
#include "rest_request_get_info.h"
#include "rest_constant.h"
#include "../../common/util.h"
#include "../../common/rest_constant.h"

using namespace stormy::common;

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

	if(IsMatch(URI, common::rest::constant::station_request_pattern))
		return new GetStation();
	if(IsMatch(URI, constant::metricsPattern))
		return new GetMetrics();
	if(IsMatch(URI, constant::meteoStationIdPattern))
		return new GetMeteo(ExtractMD5(URI));	
	if(IsMatch(URI, constant::meteoStationIdTimestampPattern))		
		return new GetMeteo(ExtractMD5(URI), "", 
			ExtractURIEndPathSegment(URI));
	if(IsMatch(URI, constant::meteoStationIdTypePattern)) {
		return new GetMeteo(ExtractMD5(URI),
			ExtractURIEndPathSegment(URI));
	}
	if(IsMatch(URI, constant::infoPattern))
		return new GetInfo("");

	return nullptr;
}
// ~~ stormy::rest::request::Factory
}}}
