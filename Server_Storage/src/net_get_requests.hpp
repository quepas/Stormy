#pragma once

#include "settings.hpp"
#include "common/net_request.hpp"

namespace stormy {
  namespace net {

struct DatabaseContext
{
  DatabaseContext(DatabaseSetting storage, DatabaseSetting aggregate)
    : db_storage(storage), db_aggregate(aggregate) {}
  DatabaseSetting db_storage;
  DatabaseSetting db_aggregate;
};

PREPARE_REQUEST(GetStationAction, DatabaseContext, GetStation)
PREPARE_REQUEST(GetMeteoAction, DatabaseContext, GetMeteo)
PREPARE_REQUEST(GetMeteoElementAction, DatabaseContext, GetMetrics)
PREPARE_REQUEST(GetAggregateAction, DatabaseContext, GetAggregate)
PREPARE_REQUEST(GetOperationAction, DatabaseContext, GetOperation)
PREPARE_REQUEST(GetPeriodAction, DatabaseContext, GetPeriod)
PREPARE_REQUEST(GetExportAction, DatabaseContext, GetExport)
PREPARE_REQUEST(GetInfoAction, DatabaseContext, GetInfo)

}}
// ~~ stormy::net::
