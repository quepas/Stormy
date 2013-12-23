#pragma once

#include "db_storage.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetInfo : public Poco::Net::HTTPRequestHandler
{
public:
  GetInfo(db::Storage* database_storage);
  ~GetInfo();

  void handleRequest(
    Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response) override;

private:
  db::Storage* database_storage_;
};
// ~~ stormy::rest::request::GetInfo
}}}