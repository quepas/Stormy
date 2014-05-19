#pragma once

#include "db_config.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo(common::db::Setting storage_setting);
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  common::db::Setting storage_setting_;
};
// ~~ stormy::rest::request::GetInfo
}}}