#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace common {
    namespace rest {
      namespace request {

class GetOptions : public Poco::Net::HTTPRequestHandler
{
public:
  GetOptions(std::string URI);
  ~GetOptions();

void handleRequest(
  Poco::Net::HTTPServerRequest& request, 
  Poco::Net::HTTPServerResponse& response) override;
private:
  std::string URI_;
};
// ~~ stormy::common::rest::request::GetOptions
}}}}
