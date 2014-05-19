#pragma once

#include "db_config.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetOperation : public Poco::Net::HTTPRequestHandler
{
public:
  GetOperation(common::db::Setting aggregate_setting);
  ~GetOperation();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  common::db::Setting aggregate_setting_;
};
// ~~ stormy::rest::request::GetOperation
}}}
