#include "DBStorage.h"

#include <exception>
#include <iostream>

#include <postgresql/soci-postgresql.h>

using namespace Stormy;
using namespace Stormy::Data;
using namespace std;
using namespace soci;

DBStorage::DBStorage( StorageDatabase* storageDB )
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

unsigned int DBStorage::countAllStation()
{
	unsigned int count = 0;
	TRY
	sql << "SELECT count(*) FROM station", into(count);
	CATCH_MSG("[StorageDB] countAllStation(): ")
	return count;
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

			sql << "INSERT INTO measurement(code, value_text, id_station, timestamp)"
				"values(:code, :value_text, :id_station, to_timestamp(:timestamp))", 
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

