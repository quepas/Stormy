#pragma once

#include "settings.hpp"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo(DatabaseSetting storage_setting);
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  DatabaseSetting storage_setting_;
};
// ~~ stormy::rest::request::GetInfo
}}}