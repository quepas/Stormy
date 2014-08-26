#pragma once

#include "db_mongo_handler.hpp"
#include "rest_uri_parser.h"

#include <string>

namespace stormy {
  namespace net {

class GetStationAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, db::MongoHandler& db_handler);
};

class GetMeteoAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, db::MongoHandler& db_handler);
};

class GetMetricsAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, db::MongoHandler& db_handler);
};

class GetInfoAction
{
public:
  static std::string PrepareResponse(common::rest::URIParser parsed_uri, db::MongoHandler& db_handler);
};

}}
// ~~ stormy::net::
