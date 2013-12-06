#pragma once

#include "rest_request_get_station.h"
#include "rest_request_get_meteo.h"
#include "rest_request_get_info.h"
#include "rest_request_bad.h"

#include "DBStorage.h"
#include "DBAggregation.h"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace rest {
    namespace request {

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  Factory(Stormy::DBStorage* db_storage, Stormy::DBAggregation* db_aggregation);
  ~Factory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) override;

private:
  Poco::Logger& logger_;

  Stormy::DBStorage* db_storage_;
  Stormy::DBAggregation* db_aggregation_;
};
// ~~ stormy::rest::request::Factory
}}}
