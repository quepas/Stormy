#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
#include "GetInfoRequest.h"
#include "GetMetricsRequest.h"
#include "../../common/util.h"
#include "rest_constant.h"

using namespace Stormy;
using namespace Stormy::REST;
using namespace std;

Stormy::GetRequestFactory::GetRequestFactory()
{

}

Stormy::GetRequestFactory::~GetRequestFactory()
{

}

HTTPRequestHandler* GetRequestFactory::createRequestHandler
	( const HTTPServerRequest& request )
{
	string URI = request.getURI();
	cout << "REST: " << URI << endl;

	if(stormy::common::IsMatch(URI, stormy::rest::constant::stationPattern))
		return new GetStationRequest();
	if(stormy::common::IsMatch(URI, stormy::rest::constant::metricsPattern))
		return new GetMetricsRequest();
	if(stormy::common::IsMatch(URI, stormy::rest::constant::meteoStationIdPattern))
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI));	
	if(stormy::common::IsMatch(URI, stormy::rest::constant::meteoStationIdTimestampPattern))		
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI), "", 
			stormy::common::ExtractURIEndPathSegment(URI));
	if(stormy::common::IsMatch(URI, stormy::rest::constant::meteoStationIdTypePattern)) {
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI),
			stormy::common::ExtractURIEndPathSegment(URI));
	}
	if(stormy::common::IsMatch(URI, stormy::rest::constant::infoPattern))
		return new GetInfoRequest("");

	return nullptr;
}
