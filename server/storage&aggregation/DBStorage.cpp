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
		sql << "INSERT INTO station(uid, name, url, refresh_time, last_update)"
			"values(:uid, :name, :url, :refresh_time, to_timestamp(0))",
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
	sql << "SELECT uid, name, url, refresh_time FROM station WHERE uid = :uid",
		into(result -> uid), into(result -> name), into(result -> url),
		into(result -> refresh_time), use(uid);
	CATCH_MSG("[StorageDB] getStationByUID(): ")
	return result;
}

bool DBStorage::existsStationByUID( string uid )
{	
  int count = 0;
	TRY
  sql << "SELECT count(*) FROM station WHERE uid = :uid",
    into(count), use(uid);
	CATCH_MSG("[StorageDB] existsStationByUID(): ")	
  return count > 0;
}

bool DBStorage::InsertMeasurements( const MeasurementPtrVector& measurements )
{
	if(!measurements.empty()) {
		TRY		
		for(auto it = measurements.begin(); it != measurements.end(); ++it) {
			string metricsCode = (*it) -> metrics -> code;
      auto metrics = GetMetricsCodes();
			if(Utils::contains(metrics, metricsCode)) {
        string station_uid = (*it)->station->uid;			
        time_t current_ts = (*it) -> timestamp.epochMicroseconds();
			  sql << "INSERT INTO measurement(code, value_text, station_uid, timestamp)"
				  "values(:code, :value_text, :station_uid, to_timestamp(:timestamp))", 
				  use(metricsCode),
				  use(boost::any_cast<std::string>((*it) -> value)), 
				  use(station_uid),
				  use(current_ts);
      
        auto station_last_update = &GetStationLastUpdate(station_uid);
        if (current_ts > mktime(station_last_update)) {          
          //cout << "Newer measure time for station uid: " << station_uid << endl;
          //cout << "\tInsertet data time: " << asctime(gmtime(&current_ts)) << endl;
          UpdateStationLastUpdate(station_uid, *gmtime(&current_ts));
          //cout << "\tStation last update: " << asctime(station_last_update) << endl;
        }
      }
		}
		return true;
		CATCH_MSG("[StorageDB] insertMeasurements(): ")			
	}	
	return false;
}

Timestamp DBStorage::findNewestMeasureTimeByStationUID( string uid )
{	
	unsigned long time = 0;
	if(!uid.empty() && existsAnyMeasurementFromStation(uid)) {
		TRY
		// TODO: fix time zone
		sql << "SELECT EXTRACT(EPOCH FROM ("
			"SELECT max(timestamp) FROM measurement WHERE "
			"station_uid = :uid) - interval '1 hour')",
			into(time), use(uid);
		CATCH_MSG("[StorageDB] findNewestMeasureTimeByStationUID(): ")
	}
	return Timestamp(time);
}

Timestamp DBStorage::findOldestMeasureTimeByStationUID( string uid )
{
	ulong time = 0;
	if(countMeasurementFromStation(uid) > 0) {
		TRY		
		// TODO: fix acquisition 'time zone' time respect
		sql << "SELECT EXTRACT(EPOCH FROM ("
			"SELECT min(timestamp) FROM measurement "
			"WHERE station_uid = :station_uid) - interval '1 hour')",
			into(time), use(uid);
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
	CATCH_MSG("[StorageDB] existsMetricsByCode(): ")
	return count > 0;
}

bool DBStorage::existsAnyMeasurementFromStation( string uid )
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement WHERE "
		"station_uid = :uid",
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

uint32 DBStorage::CountStations()
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM station", into(count);
	CATCH_MSG("[StorageDB] countStation(): ")
	return count;
}

ulong DBStorage::countMeasurementFromStation(string uid)
{
	ulong count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement "
		"WHERE uid = :uid", use(uid), into(count);
	CATCH_MSG("[StorageDB] countMeasurementFromStation() ")
	return count;
}

vector<stormy::common::entity::Station> DBStorage::GetStations()
{
	auto stations = vector<stormy::common::entity::Station>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM station");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		stormy::common::entity::Station element;
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
vector<stormy::aggregation::entity::Task> DBStorage::GetTasks()
{
	auto tasks = vector<stormy::aggregation::entity::Task>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM aggregate_task");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		stormy::aggregation::entity::Task element;
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
vector<stormy::aggregation::entity::Period> DBStorage::GetPeriods()
{
	auto periods = vector<stormy::aggregation::entity::Period>();
	TRY
	int count = 0;
	sql << "SELECT count(*) FROM aggregate_period", into(count);
	auto periods_name = vector<string>(count);
	sql << "SELECT name FROM aggregate_period", into(periods_name);
	for (auto it = periods_name.begin(); it != periods_name.end(); ++it) {
		stormy::aggregation::entity::Period element;
		element.name = *it;
		// TODO: SOCI doesn't support Postgres interval type
		element.interval = 0;
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

bool Stormy::DBStorage::DeleteTask(std::string period_name, std::string station_uid)
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

std::vector<std::string> Stormy::DBStorage::GetStationUIDs()
{
  auto station_uids = vector<string>();
  TRY
  rowset<row> rs = (sql.prepare << "SELECT uid FROM station");
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    station_uids.push_back(it->get<string>("uid"));
  }
  CATCH_MSG("[Storage] GetStationUIDs():\n\t")
  return station_uids;
}

map<time_t, string> Stormy::DBStorage::GetMeasurement(
  string station_uid, string metrics_code, tm begin, tm end)
{
  auto result = map<time_t, string>();
  TRY
    rowset<row> rs = 
    (sql.prepare << "SELECT timestamp, value_text FROM measurement "
      "WHERE station_uid = :station_uid "
      "AND code = :metrics_code AND "
      "timestamp >= :begin_time AND "
      "timestamp < :end_time", 
      use(station_uid), use(metrics_code), 
      use(begin), use(end));
  
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    result.insert(make_pair(mktime(&it->get<tm>(0)), it->get<string>(1)));
  }
  CATCH_MSG("[Storage] GetMeasurement():\n\t")
  return result;
}

map<time_t, string> DBStorage::GetMeasurementFromLast(
  string station_uid, string metrics_code, uint16_t from_last_hours )
{
  time_t time_data = time(nullptr);
  tm to_time = *localtime(&time_data);
  time_data -= from_last_hours * 3600;
  tm from_time = *localtime(&time_data);
  return GetMeasurement(station_uid, metrics_code, from_time, to_time);
}

vector<string> DBStorage::GetMetricsCodes()
{
  static auto metrics = GetMetrics();
  auto result = vector<string>(metrics.size());
  for (auto it = metrics.begin(); it != metrics.end(); ++it) {
    result.push_back(it->code);
  }
  return result;
}

vector<string> DBStorage::GetStationMeasure(string station_uid, 
  string metrics_code, tm begin_time, tm end_time)
{
  auto result = vector<string>();
  TRY
  rowset<row> rs = (sql.prepare << "SELECT value_text FROM measurement "
    "WHERE station_uid = :station_udi "
    "AND code = :metrics_code AND "
    "timestamp >= :begin_time AND "
    "timestamp < :end_time", 
    use(station_uid), use(metrics_code), 
    use(begin_time), use(end_time));

  for (auto it = rs.begin(); it != rs.end(); ++it) {
    result.push_back(it->get<string>(0));
  }
  CATCH_MSG("[Storage] GetStationMeasure():\n\t")
  return result;
}

string DBStorage::GetStationName(string uid)
{
  string result;
  TRY
  sql << "SELECT name FROM station WHERE uid = :uid", 
    use(uid), into(result);
  CATCH_MSG("[Storage] GetStationName():\n\t")
  return result;
}

tm DBStorage::GetOldestStationMeasureTime(std::string uid)
{
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM "
    "(SELECT min(timestamp) FROM measurement WHERE station_uid = :uid))",
    use(uid), into(time);
  CATCH_MSG("[Storage] GetOldestStationMeasureTime():\n\t")
  return *gmtime(&time);
}

int DBStorage::CountStationMeasures(std::string uid)
{
  int count = 0;
  TRY
  sql << "SELECT count(*) FROM measurement WHERE station_uid = :uid",
    use(uid), into(count);
  CATCH_MSG("[Storage] CountStationMeasures():\n\t")
  return count;
}

bool DBStorage::UpdateTaskCurrentTime(uint32_t id, std::tm timestamp)
{
  TRY
  sql << "UPDATE aggregate_task SET current_ts = :timestamp "
    "WHERE id = :id", use(timestamp), use(id);
  return true;
  CATCH_MSG("[Storage] UpdateTaskCurrentTime():\n\t")
  return false;
}

tm DBStorage::CalculateAggregateEndTime(string period_name, tm start_time)
{  
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM (SELECT (:start_time) + (SELECT period FROM aggregate_period "
    "WHERE name = :period_name)))", use(start_time),
    use(period_name), into(time);
  CATCH_MSG("[Storage] CalculateAggregateEndTime():\n\t")
  return *gmtime(&time);
}

bool DBStorage::UpdateStationLastUpdate(string station_uid, tm timestamp)
{
  TRY
  sql << "UPDATE station SET last_update = :last_update "
    "WHERE uid = :uid", use(timestamp), use(station_uid);
  return true;
  CATCH_MSG("[Storage] UpdateStationLastUpdate(station_uid, timestamp):\n\t")
  return false;
}

tm DBStorage::GetStationLastUpdate(string station_uid)
{
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM (SELECT last_update FROM station "
    "WHERE uid = :uid))", use(station_uid), into(time);
  CATCH_MSG("[Storage] Exception at GetStationLastDataUpdate(station_uid):\n\t")
  return *gmtime(&time);
}

bool DBStorage::CreateTask(string period_name, string station_uid)
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
