#pragma once

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(
    std::string station_uid, 
    std::string type_id = "", 
    std::string timestamp = "");
  ~GetMeteo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request,
    Poco::Net::HTTPServerResponse& response) override;
private:			
  std::string station_uid_, type_id_, timestamp_;
};
// ~~ stormy::rest::request::GetMeteo
}}}