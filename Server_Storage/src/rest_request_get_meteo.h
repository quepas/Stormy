#pragma once

#include "settings.hpp"
#include "rest_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(std::string uri, DatabaseSetting storage_setting);
  ~GetMeteo();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  DatabaseSetting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::test::request::GetMeteo
}}}