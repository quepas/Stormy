#pragma once

#include "db_mongo_handler.hpp"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace net {

class RequestFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  RequestFactory(db::MongoHandler& db_handler);
  ~RequestFactory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override;
private:
  db::MongoHandler& db_handler_;
  Poco::Logger& logger_;
};

}}
// ~~ stormy::net::RequestFactory
