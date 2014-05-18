#pragma once

#include "../../common/db_config.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetPeriod : public Poco::Net::HTTPRequestHandler
{
public:
  GetPeriod(common::db::Setting aggregate_setting);
  ~GetPeriod();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;
    
private:
  common::db::Setting aggregate_setting_;
};
// ~~ stormy::rest::request::GetPeriod
}}}
