#pragma once

#include "db_setting.h"
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
    common::db::Setting storage_setting,
    common::db::Setting aggregate_setting);
  ~GetAggregate();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  common::db::Setting storage_setting_;
  common::db::Setting aggregate_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetAggregate
}}}