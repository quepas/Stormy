#pragma once

#include "settings.hpp"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace rest {
    namespace request {

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  Factory(DatabaseSetting db_storage, DatabaseSetting db_aggregation);
  ~Factory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) override;
  
private:
  Poco::Logger& logger_;

  DatabaseSetting db_storage_;
  DatabaseSetting db_aggregation_;
};
// ~~ stormy::rest::request::Factory
}}}
