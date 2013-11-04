#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
#include "GetInfoRequest.h"
#include "Utils.h"
#include "RESTConst.h"

using namespace Stormy;
using REST::Const;

Stormy::GetRequestFactory::GetRequestFactory()
{

}

Stormy::GetRequestFactory::~GetRequestFactory()
{

}

HTTPRequestHandler* GetRequestFactory::createRequestHandler
	( const HTTPServerRequest& request )
{
	std::string URI = request.getURI();
	boost::to_lower(URI);
	std::cout << "[INFO] Requested REST API: " << URI << std::endl;
	
	if(Utils::checkTextWithRegex(URI, Const::stationPattern))
		return new GetStationRequest();
	if(Utils::checkTextWithRegex(URI, Const::meteoStationIdPattern))
		return new GetMeteoRequest(Utils::extractMD5FromText(URI));	
	if(Utils::checkTextWithRegex(URI, Const::infoPattern))
		return new GetInfoRequest(Utils::extractMD5FromText(URI));

	return nullptr;
}
