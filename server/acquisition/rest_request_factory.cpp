#include "rest_request_factory.h"

#include "rest_request_get_station.h"
#include "rest_request_get_metrics.h"
#include "rest_request_get_meteo.h"
#include "rest_request_get_info.h"
#include "rest_constant.h"
#include "../../common/util.h"

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

HTTPRequestHandler* Factory::createRequestHandler
	( const HTTPServerRequest& request )
{
  string URI = request.getURI();
  logger_.information("[rest/Factory] Handling request: " + URI);

	if(stormy::common::IsMatch(URI, constant::stationPattern))
		return new GetStation();
	if(stormy::common::IsMatch(URI, constant::metricsPattern))
		return new GetMetrics();
	if(stormy::common::IsMatch(URI, constant::meteoStationIdPattern))
		return new GetMeteo(stormy::common::ExtractMD5(URI));	
	if(stormy::common::IsMatch(URI, constant::meteoStationIdTimestampPattern))		
		return new GetMeteo(stormy::common::ExtractMD5(URI), "", 
			stormy::common::ExtractURIEndPathSegment(URI));
	if(stormy::common::IsMatch(URI, constant::meteoStationIdTypePattern)) {
		return new GetMeteo(stormy::common::ExtractMD5(URI),
			stormy::common::ExtractURIEndPathSegment(URI));
	}
	if(stormy::common::IsMatch(URI, constant::infoPattern))
		return new GetInfo("");

	return nullptr;
}
// ~~ stormy::rest::request::Factory
}}}
