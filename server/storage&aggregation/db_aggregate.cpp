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

uint32_t Aggregate::GetTaskId(string station_uid, string period_name)
{
	uint32_t id = 0;
	TRY
	sql << "SELECT id FROM aggregate_task "
		"WHERE station_uid = :station_uid "
		"AND period_name = :period_name "
		"LIMIT 1",
		use(station_uid), use(period_name), into(id);
	CATCH_MSG("[AggregationDB] getTaskId(): ")
	return id;
}

uint32_t Aggregate::InsertTask(string station_uid, stormy::aggregation::Setting aggregation)
{
	uint32_t id = 0;
	TRY
	sql << "INSERT INTO aggregate_task "
		"(station_uid, period_name, current_ts) "
		"VALUES(:station_uid, :period_name, to_timestamp(0)) RETURNING id",
		use(station_uid), use(aggregation.name), into(id);
	CATCH_MSG("[AggregationDB] insertTask(): ")
	return id;
}

string Aggregate::GetStationUIDFromTask(uint32_t task_id)
{
	string station_uid = 0;
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
