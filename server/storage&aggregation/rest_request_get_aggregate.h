#pragma once

#include "DBAggregation.h"
#include "rest_util_uri_parser.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetAggregate : public Poco::Net::HTTPRequestHandler
{
public:
  GetAggregate(std::string uri, Stormy::DBAggregation* aggregate_database);
  ~GetAggregate();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  Stormy::DBAggregation* aggregate_database_;
  util::URIParser uri_parser_;
};
// ~~ stormy::rest::request::GetAggregate
}}}