#pragma once

#include "net_request.hpp"
#include "db_mongo_handler.hpp"
#include "rest_uri_parser.h"

#include <string>

namespace stormy {
  namespace net {

struct DatabaseContext
{
  DatabaseContext(db::MongoHandler& handler) : db_handler(handler) {}
  db::MongoHandler& db_handler;
};

PREPARE_REQUEST(GetStationAction, DatabaseContext, GetStation)
PREPARE_REQUEST(GetMeteoAction,   DatabaseContext, GetMeteo)
PREPARE_REQUEST(GetMetricsAction, DatabaseContext, GetMetrics)
PREPARE_REQUEST(GetInfoAction,    DatabaseContext, GetInfo)

}}
// ~~ stormy::net::
