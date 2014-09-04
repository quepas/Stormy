#pragma once

#include "common/net_request.hpp"
#include "common/rest_uri_parser.h"
#include "db_mongo_handler.hpp"

#include <string>

namespace stormy {
  namespace net {

struct DatabaseContext
{
  db::MongoHandler& db_handler;
};

PREPARE_REQUEST(GetStationAction, DatabaseContext, GetStation)
PREPARE_REQUEST(GetMeteoAction,   DatabaseContext, GetMeteo)
PREPARE_REQUEST(GetMetricsAction, DatabaseContext, GetMetrics)
PREPARE_REQUEST(GetInfoAction,    DatabaseContext, GetInfo)

}}
// ~~ stormy::net::
