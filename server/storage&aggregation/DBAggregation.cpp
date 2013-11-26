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

DBAggregation::DBAggregation( Database* _dbAggregation, DBStorage* _dbStorage )
	:	dbAggregation(_dbAggregation),
		dbStorage(_dbStorage)
{
	connect();
}

DBAggregation::~DBAggregation()
{

}

void DBAggregation::connect()
{
	TRY
	sql.open(postgresql, dbAggregation -> asConnectionString());
	CATCH_MSG("[AggregationDB] connect(): ")
}

bool DBAggregation::insertPeriod( AggregationSetting aggregation )
{
	auto existingPeriods = getPeriodNames();
	if(!Utils::contains(existingPeriods, aggregation.name)) {
		TRY
		sql << "INSERT INTO aggregate_period VALUES(:name, :period)",
			use(aggregation.name), use(aggregation.interval);
		return true;
		CATCH_MSG("[AggregationDB] insertPeriod(): ")
	}	
	return false;
}

bool DBAggregation::insertPeriods( vector<AggregationSetting> settings )
{
	bool result = true;
	Utils::forEach(settings, [&](AggregationSetting setting) {
		if(!insertPeriod(setting))
			result = false;
	});
	return result;
}


bool DBAggregation::taskExsist( uint32 station_id, AggregationSetting aggregation )
{
	uint16 count = 0;
	TRY
	sql << "SELECT count(*) FROM aggregate_task "
		"WHERE station_id = :station_id "
		"AND period_name = :period_name ", 
		into(count), use(station_id), use(aggregation.name);		
	CATCH_MSG("[AggregationDB] taskExsist(): ")
	return count > 0;
}

uint32 DBAggregation::getTaskId( uint32 station_id, string period_name )
{
	uint32 id = 0;
	TRY
	sql << "SELECT id FROM aggregate_task "
		"WHERE station_id = :station_id "
		"AND period_name = :period_name "
		"LIMIT 1",
		use(station_id), use(period_name), into(id);
	CATCH_MSG("[AggregationDB] getTaskId(): ")
	return id;
}

uint32 DBAggregation::insertTask( uint32 station_id, AggregationSetting aggregation )
{
	uint32 id = 0;
	TRY
	sql << "INSERT INTO aggregate_task "
		"(station_id, period_name, refresh) "
		"VALUES(:station_id, :period_name, :refresh) RETURNING id",
		use(station_id), use(aggregation.name),
		use(aggregation.taskRefresh), into(id);
	CATCH_MSG("[AggregationDB] insertTask(): ")
	return id;
}

uint32 DBAggregation::createTask( uint32 station_id, AggregationSetting aggregation )
{
	uint32 task_id = 0;
	if(!taskExsist(station_id, aggregation)) {
		task_id = insertTask(station_id, aggregation);
	} else {
		task_id = getTaskId(station_id, aggregation.name);
		// refresh old (check for new or missed data)
	}
	// check for oldest measure time for station and set time
	// ~~
	return task_id;

	/*uint32 id = 0;
	TRY			
	Timestamp nowTS(1325376000);	// TODO: repair this
	cout << "Time: " << nowTS.epochMicroseconds() << endl;
	sql << "INSERT INTO aggregate_task"
		"(period, station, refresh, currenttime)"
		"VALUES(:period, :station, :refresh, to_timestamp(:currenttime))"
		"RETURNING id",
		use(aggregation.name), use(station_id),
		use(aggregation.taskRefresh),
		use(nowTS.epochMicroseconds()), into(id);
	CATCH_MSG("[AggregationDB] createTask(): ")
	return id;*/
}

vector<string> DBAggregation::getPeriodNames()
{
	uint16 count = countPeriod();	
	vector<string> result(count);
	if(count > 0) {
		TRY
		sql << "SELECT name FROM aggregate_period", into(result);
		CATCH_MSG("[AggregationDB] getPeriodNames(): ")
	}	
	return result;
}

uint32 DBAggregation::countPeriod()
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM aggregate_period", into(count);
	CATCH_MSG("[AggregationDB] countPeriod(): ")
	return count;
}

string DBAggregation::getPeriodCorrectInterval( string name )
{
	string result = "";
	TRY
	sql << "SELECT period FROM aggregate_period WHERE name = :name",
		use(name), into(result);
	CATCH_MSG("[AggregationDB]: getPeriodCorrectInterval(): ")
	return result;
}

Timestamp DBAggregation::taskCurrentTime( uint32 id )
{
	ulong resultLong = 0;
	TRY
	sql << "SELECT EXTRACT(EPOCH FROM ("
		"SELECT current_ts FROM aggregate_task WHERE id = :id))",
		use(id), into(resultLong);
	CATCH_MSG("[AggregationDB]: taskCurrentTime(): ")
	return Timestamp(resultLong);
}

void DBAggregation::increaseTaskTime( string periodName, uint32 id )
{
	TRY
	/*sql << "UPDATE aggregate_task SET currenttime = currenttime + "
		"(SELECT period FROM aggregate_period WHERE name = :name)"
		"WHERE id = :id", use(periodName), use(id);*/
	sql << "UPDATE aggregate_task SET currenttime = currenttime + interval '1 hour' "
		"WHERE id = :id", use(id);
	CATCH_MSG("[AggregationDB]: increaseTaskTime(): ")
}

