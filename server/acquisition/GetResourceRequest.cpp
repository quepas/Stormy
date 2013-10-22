#include "GetResourceRequest.h"

#include <Poco/Util/Application.h>

using namespace Stormy;

using Poco::Util::Application;

Stormy::GetResourceRequest::GetResourceRequest()
{

}

Stormy::GetResourceRequest::~GetResourceRequest()
{

}

void Stormy::GetResourceRequest::handleRequest
	( HTTPServerRequest& request, HTTPServerResponse& response )
{
	Application& app = Application::instance();
	app.logger().information("Request from: " + request.clientAddress().toString());

	std::ostream& ostr = response.send();
	ostr << "<html><head><title>Takie tam</title></head><body><h1>TAkie tAM</h1></body></html>";
}
