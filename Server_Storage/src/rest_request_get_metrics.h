#pragma once

#include "settings.hpp"
#include "rest_uri_parser.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMetrics : public Poco::Net::HTTPRequestHandler
{
public:
  GetMetrics(std::string uri, DatabaseSetting storage_setting);
  ~GetMetrics();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  DatabaseSetting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetMetrics
}}}
