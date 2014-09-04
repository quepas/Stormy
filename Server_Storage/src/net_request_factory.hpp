#pragma once

#include "net_get_requests.hpp"
#include "common/net_request_factory.hpp"

#include <Poco/Logger.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace net {

class StorageRequestFactoryAction
{
public:
  static Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request, DatabaseContext context, Poco::Logger& logger_);
};

typedef RequestFactory<StorageRequestFactoryAction, DatabaseContext> StorageRequestFactory;

}}
// ~~ stormy::net::StorageRequestFactory
