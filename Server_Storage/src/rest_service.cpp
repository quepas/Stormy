#include "rest_service.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>

#include "net_request_factory.hpp"

using std::string;
using std::vector;
using Poco::Logger;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Util::Application;

namespace stormy {
  namespace rest {

Service::Service(
  DatabaseSetting db_storage, 
  DatabaseSetting db_aggregation, 
  unsigned port /*= 8070*/)
  : port_(port),
    logger_(Logger::get("rest")),
    db_storage_(db_storage),
    db_aggregation_(db_aggregation)
{
  
}

Service::~Service()
{

}

void Service::initialize(Application& self)
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

int Service::main(const vector<string>& args)
{
  logger_.information("[rest/Service] Start");
  ServerSocket server_socket(port_);
  net::DatabaseContext context{ db_storage_, db_aggregation_ };
  HTTPServer http_server(new net::StorageRequestFactory(context),
    server_socket, new HTTPServerParams);
  http_server.start();
  waitForTerminationRequest();
  http_server.stop();
  logger_.information("[rest/Service] Stop");
  return Application::EXIT_OK;
}
// ~~ stormy::rest::Service
}}
