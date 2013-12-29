#pragma once

#include "../../common/rest_uri_parser.h"
#include "db_storage.h"

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation(std::string uri, db::Storage* storage_database);
  ~GetStation();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  db::Storage* storage_database_;
  common::rest::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetStation
}}}
