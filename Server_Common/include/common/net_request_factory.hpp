#pragma once
/*
#include <Poco/Logger.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

namespace stormy {
  namespace net {

template<typename FactoryAction, typename ActionContext>
class RequestFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  RequestFactory(const ActionContext& context)
    : context_(context),
    logger_(Poco::Logger::get("main")) {}
  ~RequestFactory() {}

  Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request) override {
    return FactoryAction::createRequestHandler(request, context_, logger_);
  };

private:
  Poco::Logger& logger_;
  const ActionContext& context_;
};

}}
// ~~ stormy::net::RequestFactory*/
