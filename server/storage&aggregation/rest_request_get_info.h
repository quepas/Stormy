#pragma once

#include "DBStorage.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo(Stormy::DBStorage* database_storage);
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  Stormy::DBStorage* database_storage_;
};
// ~~ stormy::rest::request::GetInfo
}}}