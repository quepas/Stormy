#include "GetResourceRequestFactory.h"

#include "GetResourceRequest.h"
#include <iostream>

using namespace Stormy;

Stormy::GetResourceRequestFactory::GetResourceRequestFactory()
{

}

Stormy::GetResourceRequestFactory::~GetResourceRequestFactory()
{

}

HTTPRequestHandler* GetResourceRequestFactory::createRequestHandler
	( const HTTPServerRequest& request )
{
	std::string URI = request.getURI();
	std::cout << "Request URI" << URI << std::endl;
	if(URI == "/")
		return new GetResourceRequest();
	else
		return nullptr;
}
