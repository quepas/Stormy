#pragma once

#include "db_setting.h"
#include "rest_uri_parser.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMetrics : public Poco::Net::HTTPRequestHandler
{
public:
  GetMetrics(std::string uri, common::db::Setting storage_setting);
  ~GetMetrics();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  common::db::Setting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetMetrics
}}}
