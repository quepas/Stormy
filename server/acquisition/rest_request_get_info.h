#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo(std::string station_uid = "");
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) override;

private:
  std::string station_uid_;
};
// ~~ stormy::rest::request::GetInfo
}}}
