#pragma once

#include "settings.hpp"
#include "rest_uri_parser.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation(std::string uri, DatabaseSetting storage_setting);
  ~GetStation();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  DatabaseSetting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetStation
}}}
