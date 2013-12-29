#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace common {
    namespace rest {
      namespace request {

class Bad : public Poco::Net::HTTPRequestHandler
{
public:
  Bad(std::string URI);
  ~Bad();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;
private:
  std::string URI_;
};
// ~~ stormy::common::rest::request::Bad
}}}}
