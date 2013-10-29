#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
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
	std::cout << "Request URI: " << URI << std::endl;
	
	if(Utils::checkTextWithRegex(URI, Const::stationPattern))
		return new GetStationRequest();
	if(Utils::checkTextWithRegex(URI, Const::meteoStationIdPattern)) {
		boost::regex regex("[0-9a-f]{32}");
		boost::smatch match;
		std::string stationId = Const::none;
		if(boost::regex_search(URI, match, regex)) {
			stationId = match[0];
		}		
		return new GetMeteoRequest(stationId);
	}

	return nullptr;
}
