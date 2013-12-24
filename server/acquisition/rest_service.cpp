#include "HttpServer.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>

#include "GetRequestFactory.h"

using namespace Stormy;

using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;

HttpServer::HttpServer(unsigned int _port /* = 8080 */)
	:	port(_port)
{

}

HttpServer::~HttpServer()
{

}

void HttpServer::initialized( Application& self )
{
	loadConfiguration();
	ServerApplication::initialize(self);
}

void HttpServer::unitialized()
{
	ServerApplication::uninitialize();
}

int HttpServer::main( const std::vector<std::string>& args )
{
	unsigned short configuredPort = (unsigned short)
		config().getInt("HttpServer.port", port);

	ServerSocket serverSocket(configuredPort);
	HTTPServer httpServer(new GetRequestFactory(),
		serverSocket, new HTTPServerParams);
	httpServer.start();
	waitForTerminationRequest();
	httpServer.stop();

	return Application::EXIT_OK;
}
