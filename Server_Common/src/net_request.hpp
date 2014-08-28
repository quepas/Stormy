#pragma once

#include "rest_uri_parser.h"

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

#define PREPARE_REQUEST(action_name, context_type, request_name)                                      \
    class action_name {                                                                               \
      public:                                                                                         \
        static std::string PrepareResponse(common::rest::URIParser parsed_uri, context_type context); \
      };                                                                                              \
    typedef Request<action_name, context_type> request_name;

namespace stormy {
  namespace net {

template<typename Action, typename ActionContext>
class Request : public Poco::Net::HTTPRequestHandler
{
public:
  Request(std::string uri, ActionContext context)
    : parsed_uri_(uri), context_(context) {}
  ~Request() {}

  void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override
  {
    std::ostream& ostr = response.send();
    ostr << Action::PrepareResponse(parsed_uri_, context_);
  }

private:
  common::rest::URIParser parsed_uri_;
  ActionContext context_;
};

}}
// ~~ stormy::net::Request
