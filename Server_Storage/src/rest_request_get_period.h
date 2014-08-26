#pragma once

#include "settings.hpp"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetPeriod : public Poco::Net::HTTPRequestHandler
{
public:
  GetPeriod(DatabaseSetting aggregate_setting);
  ~GetPeriod();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;
    
private:
  DatabaseSetting aggregate_setting_;
};
// ~~ stormy::rest::request::GetPeriod
}}}
