#pragma once

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace net {

template<typename ServerContext, typename RequestFactory>
class HttpServer : public Poco::Util::ServerApplication
{
public:
  HttpServer(ServerContext context, unsigned port);
  ~HttpServer();

private:
  void initialized(Poco::Util::Application& self);
  void unitialized();
  int main(const std::vector<std::string>& args);

  ServerContext context_;
  unsigned port_;
};

template<typename ServerContext, typename RequestFactory>
HttpServer<ServerContext, RequestFactory>::HttpServer(ServerContext context, unsigned port)
  : context_(context), port_(port)
{

}

template<typename ServerContext, typename RequestFactory>
HttpServer<ServerContext, RequestFactory>::~HttpServer()
{

}

template<typename ServerContext, typename RequestFactory>
void HttpServer<ServerContext, RequestFactory>::initialized(Poco::Util::Application& self)
{
  loadConfiguration();
  ServerApplication::initialize(self);
}


template<typename ServerContext, typename RequestFactory>
void HttpServer<ServerContext, RequestFactory>::unitialized()
{
  ServerApplication::uninitialize();
}

template<typename ServerContext, typename RequestFactory>
int HttpServer<ServerContext, RequestFactory>::main(const std::vector<std::string>& args)
{
  Poco::Net::ServerSocket server_socket(config().getInt("Service.port", port_));
  Poco::Net::HTTPServer http_server(
    new RequestFactory(context_),
    server_socket,
    new Poco::Net::HTTPServerParams);
  http_server.start();
  waitForTerminationRequest();
  http_server.stop();
  return Poco::Util::Application::EXIT_OK;
}

}}
// ~~ stormy::net::HttpServer
