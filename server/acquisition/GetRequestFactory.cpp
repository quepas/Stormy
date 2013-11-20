#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
#include "GetInfoRequest.h"
#include "GetMetricsRequest.h"
#include "../../common/Utils.h"
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
	cout << "[INFO]: Requested REST API: " << URI << endl;

	if(Utils::checkTextWithRegex(URI, Const::stationPattern))
		return new GetStationRequest();
	if(Utils::checkTextWithRegex(URI, Const::metricsPattern))
		return new GetMetricsRequest();
	if(Utils::checkTextWithRegex(URI, Const::meteoStationIdPattern))
		return new GetMeteoRequest(Utils::extractMD5FromText(URI));	
	if(Utils::checkTextWithRegex(URI, Const::meteoStationIdTimestampPattern))		
		return new GetMeteoRequest(Utils::extractMD5FromText(URI), "", 
			Utils::extractEndIdFromRestURI(URI));
	if(Utils::checkTextWithRegex(URI, Const::meteoStationIdTypePattern)) {
		return new GetMeteoRequest(Utils::extractMD5FromText(URI),
			Utils::extractEndIdFromRestURI(URI));
	}
	if(Utils::checkTextWithRegex(URI, Const::infoPattern))
		return new GetInfoRequest("");

	return nullptr;
}
