#include "GetMeteoRequest.h"

using namespace Stormy;

Stormy::GetMeteoRequest::GetMeteoRequest()
{

}

Stormy::GetMeteoRequest::~GetMeteoRequest()
{

}

void Stormy::GetMeteoRequest::handleRequest( HTTPServerRequest& request, HTTPServerResponse& response )
{
	std::ostream& ostr = response.send();
	ostr << "<html><head><title>Meteo</title></head><body><h1>Meteo</h1></body></html>";
}
