#pragma once

#include "db_storage.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation(db::Storage* storage_database);
  ~GetStation();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  db::Storage* storage_database_;
};
// ~~ stormy::rest::request::GetStation
}}}
