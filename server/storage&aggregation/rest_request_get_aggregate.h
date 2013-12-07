#pragma once

#include "DBAggregation.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetAggregate : public Poco::Net::HTTPRequestHandler
{
public:
  GetAggregate(Stormy::DBAggregation* aggregate_database);
  ~GetAggregate();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  Stormy::DBAggregation* aggregate_database_;
};
// ~~ stormy::rest::request::GetAggregate
}}}