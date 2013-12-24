#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
#include "GetInfoRequest.h"
#include "GetMetricsRequest.h"
#include "../../common/util.h"
#include "RESTConst.h"

using namespace Stormy;
using namespace Stormy::REST;
using REST::Const;
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

	if(stormy::common::IsMatch(URI, Const::stationPattern))
		return new GetStationRequest();
	if(stormy::common::IsMatch(URI, Const::metricsPattern))
		return new GetMetricsRequest();
	if(stormy::common::IsMatch(URI, Const::meteoStationIdPattern))
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI));	
	if(stormy::common::IsMatch(URI, Const::meteoStationIdTimestampPattern))		
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI), "", 
			stormy::common::ExtractURIEndPathSegment(URI));
	if(stormy::common::IsMatch(URI, Const::meteoStationIdTypePattern)) {
		return new GetMeteoRequest(stormy::common::ExtractMD5(URI),
			stormy::common::ExtractURIEndPathSegment(URI));
	}
	if(stormy::common::IsMatch(URI, Const::infoPattern))
		return new GetInfoRequest("");

	return nullptr;
}
