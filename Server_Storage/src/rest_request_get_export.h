#pragma once

#include "db_setting.h"
#include "rest_uri_parser.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetExport : public Poco::Net::HTTPRequestHandler
{
public:
  GetExport(std::string uri, common::db::Setting storage_setting);
  ~GetExport();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  common::db::Setting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetInfo
}}}
