#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo();
  ~GetMeteo();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);
private:
};
// ~~ stormy::test::request::GetMeteo
}}}