#include "DBAggregation.h"

#include <iostream>
#include <algorithm>
#include <postgresql/soci-postgresql.h>
#include "../../common/Utils.h"
#include <Poco/DateTime.h>

using namespace Stormy;
using namespace Data;
using namespace soci;
using namespace std;
using namespace Poco;

DBAggregation::DBAggregation( Database* database_setting, DBStorage* database_storage )
	:	database_setting_(database_setting),
		database_storage_(database_storage)
{
	Connect();
}

DBAggregation::~DBAggregation()
{

}

void DBAggregation::Connect()
{
	TRY
	sql.open(postgresql, database_setting_ -> asConnectionString());
	CATCH_MSG("[AggregationDB] connect(): ")
}

uint32_t DBAggregation::GetTaskId(string station_uid, string period_name)
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

uint32_t DBAggregation::InsertTask(string station_uid, AggregationSetting aggregation)
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

string Stormy::DBAggregation::GetStationUIDFromTask(uint32_t task_id)
{
	string station_uid = 0;
	TRY
	sql << "SELECT station_uid FROM aggregate_task "
		"WHERE id = :id", use(task_id), into(station_uid);
	CATCH_MSG("[AggregationDB] getStationIdFromTask(): ")
	return station_uid;
}

