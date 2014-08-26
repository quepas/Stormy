#include "rest_service.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>

#include "db_mongo_handler.h"
#include "net_request_factory.h"

using std::string;
using std::vector;
using Poco::Util::Application;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;

namespace stormy {
  namespace rest {

Service::Service(uint16_t port /* = 8080 */)
  : port_(port)
{

}

Service::~Service()
{

}

void Service::initialized(Application& self)
{
  loadConfiguration();
  ServerApplication::initialize(self);
}

void Service::unitialized()
{
  ServerApplication::uninitialize();
}

int Service::main(const vector<string>& args)
{
  unsigned short configuredPort = (unsigned short)
    config().getInt("Service.port", port_);

  ServerSocket serverSocket(configuredPort);
  HTTPServer httpServer(
    new net::RequestFactory(db::MongoHandler::get()),
    serverSocket,
    new HTTPServerParams);
  httpServer.start();
  waitForTerminationRequest();
  httpServer.stop();

  return Application::EXIT_OK;
}
// ~~ stormy::rest::Service
}}
