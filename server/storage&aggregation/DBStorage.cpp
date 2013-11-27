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
			use(station -> url), use(station -> refreshTime);
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
		into(result -> refreshTime), use(uid);
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

