#pragma once

#include "../../common/db_setting.h"
#include "../../common/rest_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(std::string uri, common::db::Setting storage_setting);
  ~GetMeteo();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  common::db::Setting storage_setting_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::test::request::GetMeteo
}}}