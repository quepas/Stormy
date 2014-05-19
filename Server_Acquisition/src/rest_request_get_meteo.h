#pragma once

#include "rest_uri_parser.h"
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(std::string uri);
  ~GetMeteo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) override;
private:			
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetMeteo
}}}