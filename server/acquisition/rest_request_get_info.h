#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo();
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) override;
};
// ~~ stormy::rest::request::GetInfo
}}}
