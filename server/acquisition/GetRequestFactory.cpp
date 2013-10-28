#include "GetRequestFactory.h"

#include <boost/algorithm/string.hpp>
#include "GetStationRequest.h"
#include "GetMeteoRequest.h"
#include <iostream>

using namespace Stormy;

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
	
	if(boost::contains(URI, "station"))
		return new GetStationRequest();
	if(boost::contains(URI, "meteo"))
		return new GetMeteoRequest();

	return nullptr;
}
