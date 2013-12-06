#pragma once

#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace rest {
    namespace request {

class Factory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  Factory();
  ~Factory();

  Poco::Net::HTTPRequestHandler* createRequestHandler(
    const Poco::Net::HTTPServerRequest& request) override;

private:
  Poco::Logger& logger_;
};
// ~~ stormy::rest::request::Factory
}}}
