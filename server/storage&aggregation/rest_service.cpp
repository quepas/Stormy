#include "rest_service.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>

#include "rest_request_factory.h"

using Poco::Logger;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;

namespace stormy {
  namespace rest {

Service::Service(uint16_t port /* = 8070 */)
  : port_(port),
    logger_(Logger::get("rest"))
{
  
}

Service::~Service()
{

}

void Service::initialize(Poco::Util::Application& self)
{  
  loadConfiguration();
  ServerApplication::initialize(self);
  logger_.information("[rest/Service] Initialized");
}

void Service::uninitialize()
{
  ServerApplication::uninitialize();
  logger_.information("[rest/Service] Uninitialized");
}

int Service::main(const std::vector<std::string>& args)
{
  logger_.information("[rest/Service] Start");
  ServerSocket server_socket(port_);
  HTTPServer http_server(new request::Factory(), server_socket, 
    new HTTPServerParams);
  http_server.start();
  waitForTerminationRequest();
  http_server.stop();
  logger_.information("[rest/Service] Stop");
  return Application::EXIT_OK;
}
// ~~ stormy::rest::Service
}}
