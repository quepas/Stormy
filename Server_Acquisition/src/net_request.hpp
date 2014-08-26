#pragma once

#include "db_mongo_handler.hpp"
#include "rest_uri_parser.h"

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace net {

template<typename Action, typename ActionContext>
class Request : public Poco::Net::HTTPRequestHandler
{
public:
  Request(std::string uri, ActionContext context)
    : parsed_uri_(uri), context_(context) {}
  ~Request() {}

  void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
  common::rest::URIParser parsed_uri_;
  ActionContext context_;
};

template<typename Action, typename ActionContext>
void Request<Action, ActionContext>::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  std::ostream& ostr = response.send();
  ostr << Action::PrepareResponse(parsed_uri_, context_);
}

}}
// ~~ stormy::net::Request
