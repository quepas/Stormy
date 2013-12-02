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
	Connect();
}

DBAggregation::~DBAggregation()
{

}

void DBAggregation::Connect()
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


bool DBAggregation::taskExsist( string station_uid, AggregationSetting aggregation )
{
	uint16 count = 0;
	TRY
	sql << "SELECT count(*) FROM aggregate_task "
		"WHERE station_uid = :station_uid "
		"AND period_name = :period_name ", 
		into(count), use(station_uid), use(aggregation.name);		
	CATCH_MSG("[AggregationDB] taskExsist(): ")
	return count > 0;
}

uint32 DBAggregation::GetTaskId(string station_uid, string period_name)
{
	uint32 id = 0;
	TRY
	sql << "SELECT id FROM aggregate_task "
		"WHERE station_uid = :station_uid "
		"AND period_name = :period_name "
		"LIMIT 1",
		use(station_uid), use(period_name), into(id);
	CATCH_MSG("[AggregationDB] getTaskId(): ")
	return id;
}

uint32 DBAggregation::InsertTask(string station_uid, AggregationSetting aggregation)
{
	uint32 id = 0;
	TRY
	sql << "INSERT INTO aggregate_task "
		"(station_uid, period_name, current_ts) "
		"VALUES(:station_uid, :period_name, to_timestamp(0)) RETURNING id",
		use(station_uid), use(aggregation.name), into(id);
	CATCH_MSG("[AggregationDB] insertTask(): ")
	return id;
}

uint32 DBAggregation::createOrRefreshTask(string station_uid, AggregationSetting aggregation)
{
	uint32 task_id = 0;
	if(!taskExsist(station_uid, aggregation)) {
		task_id = InsertTask(station_uid, aggregation);
	} else {
		task_id = GetTaskId(station_uid, aggregation.name);		
	}
	// task never run over some data
	if(3600 == taskCurrentTime(task_id).epochMicroseconds()) {		
		if(getStorageDatabase() -> 
			countMeasurementFromStation(station_uid) > 0) {
				// TODO: count proper start time for other intervals
				setTaskTime(task_id, getStorageDatabase() -> 
					findOldestMeasureTimeByStationUID(station_uid));
		}
	}	
	return task_id;
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

Timestamp DBAggregation::getPeriodAsSecond( string period_name )
{
	ulong time = 0;
	TRY	
	sql << "SELECT date_part('epoch', ("
		"SELECT period FROM aggregate_period "
		"WHERE name = :name)::interval)", 
		use(period_name), into(time);
	CATCH_MSG("[AggregationDB] getPeriodAsSecond(): ")
	return Timestamp(time);
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
	ulong time = 0;
	TRY
	sql << "SELECT EXTRACT(EPOCH FROM ("
		"SELECT current_ts FROM aggregate_task "
		"WHERE id = :id))",
		use(id), into(time);
	CATCH_MSG("[AggregationDB]: taskCurrentTime(): ")
	return Timestamp(time);
}

bool DBAggregation::setTaskTime( uint32 id, Timestamp time )
{
	TRY
	sql << "UPDATE aggregate_task "
		"SET current_ts = to_timestamp(:time) "
		"WHERE id = :id",
		use(time.epochMicroseconds()), use(id);
	return true;
	CATCH_MSG("[AggregationDB] setTaskTime(): ")
	return false;
	
}

bool DBAggregation::increaseTaskTimeBySeconds( uint32 id, ulong time )
{
	string timeAsSeconds("\'");	
	timeAsSeconds.append(lexical_cast<std::string>(time));
	timeAsSeconds.append(" seconds\'");
	TRY
	sql << "UPDATE aggregate_task "
	"SET current_ts = current_ts + :time ::interval  "
		"WHERE id = :id",
		use(timeAsSeconds), use(id);
	return true;
	CATCH_MSG("[AggregationDB] increaseTaskTimeBySeconds(): ")
	return false;
}

string Stormy::DBAggregation::GetStationUIDFromTask(uint32 id)
{
	string station_uid = 0;
	TRY
	sql << "SELECT station_uid FROM aggregate_task "
		"WHERE id = :id", use(id), into(station_uid);
	CATCH_MSG("[AggregationDB] getStationIdFromTask(): ")
	return station_uid;
}

