#pragma once

#include "db_storage.h"
#include "rest_util_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

/*
 *  Getting meteo data from last N hours (24 by default)
 */
class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(std::string uri, db::Storage* storage_database);
  ~GetMeteo();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  db::Storage* storage_database_;
  util::URIParser uri_parser_;
};
// ~~ stormy::test::request::GetMeteo
}}}