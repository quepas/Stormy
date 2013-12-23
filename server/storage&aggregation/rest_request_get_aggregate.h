#pragma once

#include "db_aggregate.h"
#include "rest_util_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetAggregate : public Poco::Net::HTTPRequestHandler
{
public:
  GetAggregate(std::string uri, db::Aggregate* aggregate_database);
  ~GetAggregate();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  db::Aggregate* aggregate_database_;
  util::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetAggregate
}}}