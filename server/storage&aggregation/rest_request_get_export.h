#pragma once

#include "../../common/rest_uri_parser.h"
#include "db_storage.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetExport : public Poco::Net::HTTPRequestHandler
{
public:
  GetExport(std::string uri, db::Storage* database_storage);
  ~GetExport();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  db::Storage* database_storage_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetInfo
}}}
