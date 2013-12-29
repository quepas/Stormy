#pragma once

#include "../../common/rest_uri_parser.h"
#include "db_storage.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMetrics : public Poco::Net::HTTPRequestHandler
{
public:
  GetMetrics(std::string uri, db::Storage* storage_database);
  ~GetMetrics();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  db::Storage* storage_database_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetMetrics
}}}
