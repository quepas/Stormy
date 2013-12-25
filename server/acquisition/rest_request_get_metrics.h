#pragma once

#include <string>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMetrics : public Poco::Net::HTTPRequestHandler
{
public:
  GetMetrics(std::string station_uid = "");
  ~GetMetrics();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  std::string station_uid_;
};
// ~~ stormy::rest::request::GetMetrics
}}}
