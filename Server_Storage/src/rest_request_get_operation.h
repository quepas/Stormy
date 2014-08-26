#pragma once

#include "settings.hpp"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetOperation : public Poco::Net::HTTPRequestHandler
{
public:
  GetOperation(DatabaseSetting aggregate_setting);
  ~GetOperation();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  DatabaseSetting aggregate_setting_;
};
// ~~ stormy::rest::request::GetOperation
}}}
