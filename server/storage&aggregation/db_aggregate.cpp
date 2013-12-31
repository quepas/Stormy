#include "db_aggregate.h"

#include <iostream>
#include <postgresql/soci-postgresql.h>

using Poco::Logger;
using soci::postgresql;
using soci::use;
using soci::into;

namespace stormy {
  namespace db {

Aggregate::Aggregate( common::db::Setting* database_setting, Storage* database_storage )
	:	logger_(Logger::get("aggregation")),
    database_setting_(database_setting),
		database_storage_(database_storage)
{
	Connect();
}

Aggregate::~Aggregate()
{

}

void Aggregate::Connect()
{
	TRY
	sql.open(postgresql, database_setting_ -> AsConnectionString());
	CATCH_MSG("[AggregationDB] connect(): ")
}

string Aggregate::GetStationUIDFromTask(uint32_t task_id)
{
	string station_uid = "";
	TRY
	sql << "SELECT station_uid FROM aggregate_task "
		"WHERE id = :id", use(task_id), into(station_uid);
	CATCH_MSG("[AggregationDB] getStationIdFromTask(): ")
	return station_uid;
}

tm Aggregate::GetTaskCurrentTS(uint32_t task_id)
{
  tm timestamp;
  TRY
  sql << "SELECT current_ts FROM aggregate_task "
    "WHERE id = :id", use(task_id), into(timestamp);
  CATCH_MSG("[AggregationDB] GetTaskCurrentTS():\n\t")
  return timestamp;
}

bool Aggregate::InsertAggregate(stormy::aggregation::entity::Aggregate aggregate)
{
  TRY
  sql << "INSERT INTO aggregate (station_uid, metrics_code, "
    "operation_name, period_name, start_time, value, sample_number) "
    "VALUES(:station_uid, "
    ":metrics_code, :operation_name, :period_name, "
    ":start_time, :value, :sample_number)",
    use(aggregate.station_uid), use(aggregate.metrics_code),
    use(aggregate.operation_name), use(aggregate.period_name),
    use(aggregate.start_time), use(aggregate.value), 
    use(aggregate.sample_number);
  return true;
  CATCH_MSG("[AggregationDB] InsertAggregate():\n\t")
  return false;
}
// ~~ stormy::db::Aggregate
}}
