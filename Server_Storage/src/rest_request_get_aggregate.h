#pragma once

#include "settings.hpp"
#include "rest_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetAggregate : public Poco::Net::HTTPRequestHandler
{
public:
  GetAggregate(
    std::string uri, 
    DatabaseSetting storage_setting,
    DatabaseSetting aggregate_setting);
  ~GetAggregate();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  DatabaseSetting storage_setting_;
  DatabaseSetting aggregate_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetAggregate
}}}