#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation();
  ~GetStation();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);
};
// ~~ stormy::rest::request::GetStation
}}}
