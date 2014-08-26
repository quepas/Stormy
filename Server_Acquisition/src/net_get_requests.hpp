#pragma once

#include "net_request.hpp"
#include "db_mongo_handler.hpp"
#include "rest_uri_parser.h"

#include <string>

namespace stormy {
  namespace net {

class GetStationAction;
class GetMeteoAction;
class GetMetricsAction;
class GetInfoAction;

struct DatabaseContext
{
  DatabaseContext(db::MongoHandler& handler) : db_handler(handler) {}
  db::MongoHandler& db_handler;
};

typedef Request<GetStationAction, DatabaseContext> GetStation;
typedef Request<GetMeteoAction, DatabaseContext> GetMeteo;
typedef Request<GetMetricsAction, DatabaseContext> GetMetrics;
typedef Request<GetInfoAction, DatabaseContext> GetInfo;

class GetStationAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, DatabaseContext db_context);
};

class GetMeteoAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, DatabaseContext db_context);
};

class GetMetricsAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, DatabaseContext db_context);
};

class GetInfoAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, DatabaseContext db_context);
};

}}
// ~~ stormy::net::
