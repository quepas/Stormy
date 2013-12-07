#pragma once

#include "DBStorage.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetStation : public Poco::Net::HTTPRequestHandler
{
public:
  GetStation(Stormy::DBStorage* storage_database);
  ~GetStation();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  Stormy::DBStorage* storage_database_;
};
// ~~ stormy::rest::request::GetStation
}}}
