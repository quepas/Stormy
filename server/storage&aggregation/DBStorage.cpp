#include "DBStorage.h"

#include <exception>
#include <iostream>

#include <postgresql/soci-postgresql.h>

using namespace Stormy;
using namespace Stormy::Data;
using namespace std;
using namespace soci;
using namespace Poco;

DBStorage::DBStorage( Database* storageDB )
	:	configuration(storageDB)
{	
	connect();
}

DBStorage::~DBStorage()
{

}

void DBStorage::connect()
{
	TRY
	sql.open(postgresql, configuration -> asConnectionString());	
	CATCH_MSG("[StorageDB] connect(): ")
}

void DBStorage::insertStation( StationPtr station )
{
	if(station) {
		TRY
		sql << "INSERT INTO station(uid, name, url, refresh)"
			"values(:uid, :name, :url, :refresh)",
			use(station -> uid), use(station -> name), 
			use(station -> url), use(station -> refresh_time);
		CATCH_MSG("[StorageDB] insertStation(): ")		
	}
}

void DBStorage::insertStations( const StationPtrVector& stations )
{
	Utils::forEach(stations, [&](StationPtr station) {
		if(!existsStationByUID(station -> uid))
			insertStation(station);
	});
}


void DBStorage::clearAllStation()
{
	TRY
	sql << "DELETE FROM station";
	CATCH_MSG("[StorageDB] clearAllStation(): ")
}

Station* DBStorage::getStationByUID( string uid )
{
	Station* result = new Station();
	TRY
	sql << "SELECT uid, name, url, refresh FROM station WHERE uid = :uid",
		into(result -> uid), into(result -> name), into(result -> url),
		into(result -> refresh_time), use(uid);
	CATCH_MSG("[StorageDB] getStationByUID(): ")
	return result;
}

bool DBStorage::existsStationByUID( string uid )
{
	unsigned int count = 0;
	TRY
	sql << "SELECT count(*) FROM station WHERE uid = :uid",
		into(count), use(uid);
	CATCH_MSG("[StorageDB] existsStationByUID(): ")
	return count > 0;
}


uint32 DBStorage::getStationIdByUID( string uid )
{
	uint32 id = 0;
	TRY
	sql << "SELECT id FROM station WHERE uid = :uid",
		into(id), use(uid);
	CATCH_MSG("[StorageDB] getStationIdByUID(): ")
	return id;
}


bool DBStorage::insertMeasurements( const MeasurementPtrVector& measurements )
{
	if(!measurements.empty()) {
		TRY		
		for(auto it = measurements.begin(); it != measurements.end(); ++it) {
			std::string metricsCode = (*it) -> metrics -> code;
			metricsCode = existsMetricsByCode(metricsCode) ? metricsCode : "unknown";
			uint32 stationId = getStationIdByUID((*it) -> station -> uid);

			sql << "INSERT INTO measurement(code, value_text, station, timestamp)"
				"values(:code, :value_text, :station, to_timestamp(:timestamp))", 
				use(metricsCode),
				use(boost::any_cast<std::string>((*it) -> value)), 
				use(stationId),
				use((*it) -> timestamp.epochMicroseconds());
		}
		return true;
		CATCH_MSG("[StorageDB] insertMeasurements(): ")			
	}	
	return false;
}

Timestamp DBStorage::findNewestMeasureTimeFromStation( uint32 id )
{
	ulong time = 0;
	if(countMeasurementFromStation(id) > 0) {
		TRY
		// TODO: fix time zone
		sql << "SELECT EXTRACT(EPOCH FROM ("
			"SELECT max(timestamp) FROM measurement "
			"WHERE station = :id) - interval '1 hour')",
			into(time), use(id);
		CATCH_MSG("[StorageDB] findNewestMeasureTimeByStationUID(): ")
	}
	return Timestamp(time);
}

Timestamp DBStorage::findNewestMeasureTimeByStationUID( string uid )
{	
	unsigned long time = 0;
	if(!uid.empty() && existsAnyMeasurementFromStation(uid)) {
		TRY
		// TODO: fix time zone
		sql << "SELECT EXTRACT(EPOCH FROM ("
			"SELECT max(timestamp) FROM measurement WHERE "
			"station = (SELECT id FROM station WHERE uid = :uid)) - interval '1 hour')",
			into(time), use(uid);
		CATCH_MSG("[StorageDB] findNewestMeasureTimeByStationUID(): ")
	}
	return Timestamp(time);
}

Timestamp DBStorage::findOldestMeasureTimeByStationUID( uint32 id )
{
	ulong time = 0;
	if(countMeasurementFromStation(id) > 0) {
		TRY		
		// TODO: fix acquisition 'time zone' time respect
		sql << "SELECT EXTRACT(EPOCH FROM ("
			"SELECT min(timestamp) FROM measurement "
			"WHERE station = :id) - interval '1 hour')",
			into(time), use(id);
		CATCH_MSG("[StorageDB] findNewestMeasureTimeByStationUID(): ")
	}
	return Timestamp(time);
}


bool DBStorage::insertOneMetrics( const MetricsPtr& metrics )
{
	if(metrics) {
		TRY
		sql << "INSERT INTO metrics(code, equivalents, type, unit, format)"
			"values(:code, :equivalents, :type, :unit, :format)",
			use(metrics -> code), use(metrics -> equivalents),
			use(metrics -> value_type), use(metrics -> value_unit),
			use(metrics -> value_format);
		return true;
		CATCH			
	}
	return false;	
}

bool DBStorage::insertMetrics( const MetricsPtrVector& metrics )
{
	if(!metrics.empty()) {
		Utils::forEach(metrics, [&](MetricsPtr metric) {
			if(!existsMetricsByCode(metric -> code))
				insertOneMetrics(metric);
		});
	}
	return true;
}

bool DBStorage::existsMetricsByCode( const string& code )
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM metrics WHERE code = :code",
		into(count), use(code);
	CATCH_MSG("[StorageDB] existsStationByUID(): ")
	return count > 0;
}

bool DBStorage::existsAnyMeasurementFromStation( string uid )
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement WHERE "
		"station = (SELECT id FROM station WHERE uid = :uid)",
		into(count), use(uid);
	CATCH_MSG("[StorageDB] existsAnyMeasurementFromStation(): ")
	return count > 0;
}

uint32 DBStorage::countAllMeasurements()
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement", into(count);
	CATCH_MSG("[StorageDB] countAllMeasurements(): ")
	return count;
}

vector<uint32> DBStorage::getStationIds()
{
	uint16 count = countStation();
	vector<uint32> result(count);
	if(count > 0) {
		TRY
		sql << "SELECT id FROM station", into(result);
		CATCH_MSG("[StorageDB] getStationIds()")
	}	
	return result;
}

uint32 DBStorage::countStation()
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM station", into(count);
	CATCH_MSG("[StorageDB] countStation(): ")
	return count;
}

string DBStorage::getStationName( uint32 id )
{
	string result;
	TRY
	sql << "SELECT name FROM station WHERE id = :id",
		use(id), into(result);
	CATCH_MSG("[StorageDB] getStationName(): ")
	return result;
}

ulong DBStorage::countMeasurementFromStation( uint32 id )
{
	ulong count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement "
		"WHERE id = :id", use(id), into(count);
	CATCH_MSG("[StorageDB] countMeasurementFromStation() ")
	return count;
}

vector<Station> DBStorage::GetStations()
{
	auto stations = vector<Station>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM station");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		Station element;
		element.uid = row.get<string>(0);
		element.name = row.get<string>(1);
		element.url = row.get<string>(2);
		element.refresh_time = row.get<int>(3);
		element.last_update = row.get<tm>(4);
		stations.push_back(element);
	}
	CATCH_MSG("[Storage] Exception at GetStations():\n\t")
	return stations;
}

vector<Metrics> DBStorage::GetMetrics()
{
	auto metrics = vector<Metrics>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM metrics");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		Metrics element;
		element.code = row.get<string>(0);
		element.equivalents = row.get<string>(1);
		element.value_type = row.get<string>(2);
		element.value_unit = row.get<string>(3);
		element.value_format = row.get<string>(4);
		metrics.push_back(element);
	}
	CATCH_MSG("[Storage] Exception at GetMetrics():\n\t")
	return metrics;
}

// TODO: move to DBAggregate?
vector<stormy::aggregate::entity::Task> DBStorage::GetTasks()
{
	auto tasks = vector<stormy::aggregate::entity::Task>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM aggregate_task");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		stormy::aggregate::entity::Task element;
		element.id = row.get<int>(0);
		element.period_name = row.get<string>(1);
		element.station_uid = row.get<string>(2);		
		element.current_ts = row.get<tm>(3);
		tasks.push_back(element);
	}
	CATCH_MSG("[Storage] Exception at GetTasks():\n\t")
	return tasks;
}

// TODO: move to DBAggregate?
vector<stormy::aggregate::entity::Period> DBStorage::GetPeriods()
{
	auto periods = vector<stormy::aggregate::entity::Period>();
	TRY
	int count = 0;
	sql << "SELECT count(*) FROM aggregate_period", into(count);
	auto periods_name = vector<string>(count);
	sql << "SELECT name FROM aggregate_period", into(periods_name);
	for (auto it = periods_name.begin(); it != periods_name.end(); ++it) {
		stormy::aggregate::entity::Period element;
		element.name = *it;
		// TODO: SOCI doesn't support Postgres interval type
		element.interval = tm();
		periods.push_back(element);
	}	
	CATCH_MSG("[Storage] Exception at GetPeriods():\n\t")
	return periods;
}

bool DBStorage::DeleteTask( int id )
{
	TRY
	sql << "DELETE FROM aggregate_task WHERE id = :id", use(id);
	return true;
	CATCH_MSG("[Storage] Exception at DeleteTask():\n\t")
	return false;
}

bool Stormy::DBStorage::DeleteTask( std::string period_name, std::string station_uid )
{
	TRY
	sql << "DELETE FROM aggregate_task "
		"WHERE period_name = :period_name "
		"AND station_uid = :station_uid",
		use(period_name), use(station_uid);
	return true;
	CATCH_MSG("[Storage] Exception at DeleteTask(period, station):\n\t")
	return false;
}

bool DBStorage::CreateTask( string period_name, string station_uid )
{
	TRY
	sql << "INSERT INTO aggregate_task "
		"(period_name, station_uid, current_ts)"
		"VALUES(:period_name, :station_uid, to_timestamp(0))",
		use(period_name), use(station_uid);
	return true;
	CATCH_MSG("[Storage] CreateTask(period, station):\n\t")
	return false;
}
