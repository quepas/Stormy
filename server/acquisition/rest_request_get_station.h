#pragma once

#include "../../common/rest_uri_parser.h"
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation(std::string uri);
  ~GetStation();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) override;

private:
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetStation
}}}
