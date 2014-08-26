#include "net_http_server.h"
#include "net_request_factory.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>

using std::string;
using std::vector;
using Poco::Util::Application;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;

namespace stormy {
  namespace net {

HTTPServer::HTTPServer(db::MongoHandler& db_handler, unsigned port /* = 8080 */)
  : db_handler_(db_handler),
    port_(port)
{

}

HTTPServer::~HTTPServer()
{

}

void HTTPServer::initialized(Application& self)
{
  loadConfiguration();
  ServerApplication::initialize(self);
}

void HTTPServer::unitialized()
{
  ServerApplication::uninitialize();
}

int HTTPServer::main(const vector<string>& args)
{
  ServerSocket serverSocket(config().getInt("Service.port", port_));
  Poco::Net::HTTPServer httpServer(
    new RequestFactory(db_handler_),
    serverSocket,
    new HTTPServerParams);
  httpServer.start();
  waitForTerminationRequest();
  httpServer.stop();
  return Application::EXIT_OK;
}

}}
// ~~ stormy::net::HTTPServer