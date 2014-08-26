#pragma once

#include "db_mongo_handler.h"
#include "net_get_requests.hpp"
#include "rest_uri_parser.h"

#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPRequestHandler.h>

namespace stormy {
  namespace net {

template<typename Action>
class Request;

typedef Request<GetStationAction> GetStation;
typedef Request<GetMeteoAction> GetMeteo;
typedef Request<GetMetricsAction> GetMetrics;
typedef Request<GetInfoAction> GetInfo;

template<typename Action>
class Request : public Poco::Net::HTTPRequestHandler
{
public:
  Request(std::string uri, db::MongoHandler& db_handler) 
    : parsed_uri_(uri), db_handler_(db_handler) {}
  ~Request() {}

  void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;

private:
  common::rest::URIParser parsed_uri_;
  db::MongoHandler& db_handler_;
};

template<typename Action>
void Request<Action>::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  std::ostream& ostr = response.send();
  ostr << Action::PrepareResponse(parsed_uri_, db_handler_);
}

}}
// ~~ stormy::net::Request
