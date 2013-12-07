#pragma once

#include "DBStorage.h"

#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace rest {
    namespace request {

class GetMeteo : public Poco::Net::HTTPRequestHandler
{
public:
  GetMeteo(Stormy::DBStorage* storage_database);
  ~GetMeteo();

  void handleRequest(Poco::Net::HTTPServerRequest& request, 
    Poco::Net::HTTPServerResponse& response);

private:
  Stormy::DBStorage* storage_database_;
};
// ~~ stormy::test::request::GetMeteo
}}}