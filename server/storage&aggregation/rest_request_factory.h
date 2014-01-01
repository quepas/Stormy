#pragma once

#include "../../common/db_setting.h"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace rest {
    namespace request {

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  Factory(common::db::Setting db_storage, common::db::Setting db_aggregation);
  ~Factory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) override;
  
private:
  Poco::Logger& logger_;

  common::db::Setting db_storage_;
  common::db::Setting db_aggregation_;
};
// ~~ stormy::rest::request::Factory
}}}
