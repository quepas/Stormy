#pragma once

#include "db_storage.h"
#include "db_aggregate.h"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace rest {
    namespace request {

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  Factory(db::Storage* db_storage, db::Aggregate* db_aggregation);
  ~Factory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) override;
  
private:
  Poco::Logger& logger_;

  db::Storage* db_storage_;
  db::Aggregate* db_aggregation_;
};
// ~~ stormy::rest::request::Factory
}}}
