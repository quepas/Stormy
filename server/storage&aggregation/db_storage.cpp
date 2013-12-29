#include "db_storage.h"

#include <exception>
#include <iostream>

#include <boost/range/algorithm/copy.hpp>
#include <postgresql/soci-postgresql.h>

using namespace stormy::common;

using boost::copy;
using std::back_inserter;
using std::map;
using std::string;
using std::tm;
using std::time_t;
using std::vector;
using Poco::Logger;
using Poco::Timestamp;
using soci::postgresql;
using soci::use;
using soci::into;
using soci::row;
using soci::rowset;

namespace stormy {
  namespace db {

Storage::Storage(common::db::Setting* storageDB)
	:	logger_(Logger::get("aggregation")),
    configuration(storageDB)
{	
	connect();
}

Storage::~Storage()
{

}

void Storage::connect()
{
	TRY
	sql.open(postgresql, configuration -> AsConnectionString());	
	CATCH_MSG("[StorageDB] connect(): ")
}

void Storage::insertStation(entity::Station station)
{
  TRY
  sql << "INSERT INTO station(uid, name, url, refresh_time, last_update)"
    "values(:uid, :name, :url, :refresh_time, to_timestamp(0))",
    use(station.uid), use(station.name), 
    use(station.url), use(station.refresh_time);
  CATCH_MSG("[StorageDB] insertStation(): ")
}

void Storage::insertStations(const vector<entity::Station>& stations)
{
	Each(stations, [&](entity::Station station) {
		if(!existsStationByUID(station.uid))
			insertStation(station);
	});
}

entity::Station Storage::getStationByUID(string uid)
{
	entity::Station station;
	TRY
	sql << "SELECT uid, name, url, refresh_time FROM station WHERE uid = :uid",
		into(station.uid), into(station.name), into(station.url),
		into(station.refresh_time), use(uid);
	CATCH_MSG("[StorageDB] getStationByUID(): ")
	return station;
}

bool Storage::existsStationByUID(string uid)
{	
  int count = 0;
	TRY
  sql << "SELECT count(*) FROM station WHERE uid = :uid",
    into(count), use(uid);
	CATCH_MSG("[StorageDB] existsStationByUID(): ")	
  return count > 0;
}

void Storage::InsertMeasureSets(
  const map<time_t, vector<entity::Measurement>>& measure_sets)
{
  vector<entity::Measurement> measures;
  for (auto it = measure_sets.begin(); it != measure_sets.end(); ++it) {
    copy(it->second, back_inserter(measures));
  }
  for (auto m_it = measures.begin(); m_it != measures.end(); ++m_it) {
    if (m_it->is_numeric)
      InsertMeasureAsNumeric(*m_it);
    else
      InsertMeasureAsText(*m_it);
  }
}

void Storage::UpdateStationLastUpdateIfNeeded(
  string station_uid,
  tm last_update)
{
  time_t station_last_update = mktime(&GetStationLastUpdate(station_uid));
  time_t measure_last_update = mktime(&last_update);
  if (measure_last_update > station_last_update) {
    TRY
    sql << "UPDATE station SET last_update = :last_update "
      "WHERE uid = :uid", use(last_update), use(station_uid);
    CATCH_MSG("[db/Storage] UpdateStationLastUpdateIfNeeded: ")
  }
}


void Storage::InsertMeasureAsText(const entity::Measurement& measure)
{
  TRY  
  sql << "INSERT INTO measurement(code, value_text, station_uid, timestamp)"
    "values(:code, :value_text, :station_uid, :timestamp", 
    use(measure.code),
    use(measure.value_text), 
    use(measure.station_uid),
    use(measure.timestamp);
  CATCH_MSG("[db/Storage] InsertMeasureAsText: ")
  UpdateStationLastUpdateIfNeeded(measure.station_uid, measure.timestamp);
}

void Storage::InsertMeasureAsNumeric(const entity::Measurement& measure)
{
  TRY  
  sql << "INSERT INTO measurement(code, value_number, station_uid, timestamp)"
    "values(:code, :value_number, :station_uid, :timestamp)", 
    use(measure.code),
    use(measure.value_number), 
    use(measure.station_uid),
    use(measure.timestamp);
  CATCH_MSG("[db/Storage] InsertMeasureAsText: ")
  UpdateStationLastUpdateIfNeeded(measure.station_uid, measure.timestamp);
}

tm Storage::findNewestMeasureTimeByStationUID(string station_uid)
{	
  tm newest_measure_time;
	if(CountStationMeasurements(station_uid) > 0) {
		TRY
    sql << "SELECT max(timestamp) FROM measurement "
      "WHERE station_uid = :station_uid",
      into(newest_measure_time), use(station_uid);
		CATCH_MSG("[db/Storage] findNewestMeasureTimeByStationUID: ")
	}
	return newest_measure_time;
}

Timestamp Storage::findOldestMeasureTimeByStationUID(string uid)
{
	time_t time = 0;
	if(CountStationMeasurements(uid) > 0) {
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


bool Storage::insertOneMetrics(entity::Metrics metrics)
{
  TRY
  sql << "INSERT INTO metrics(code, equivalents, type, unit, format)"
    "values(:code, :equivalents, :type, :unit, :format)",
    use(metrics.code), use(metrics.equivalents),
    use(metrics.type), use(metrics.unit),
    use(metrics.format);
  return true;
  CATCH			
	return false;	
}

bool Storage::insertMetrics(const vector<entity::Metrics>& metrics)
{
	if(!metrics.empty()) {
		Each(metrics, [&](entity::Metrics metric) {
			if(!existsMetricsByCode(metric.code))
				insertOneMetrics(metric);
		});
	}
	return true;
}

bool Storage::existsMetricsByCode(const string& code)
{
	uint32_t count = 0;
	TRY
	sql << "SELECT count(*) FROM metrics WHERE code = :code",
		into(count), use(code);
	CATCH_MSG("[StorageDB] existsMetricsByCode(): ")
	return count > 0;
}

uint64_t Storage::CountAllMeasurements()
{
	uint64_t count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement", into(count);
	CATCH_MSG("[db/Storage] CountAllMeasurements: ")
	return count;
}

uint32_t Storage::CountAllStations()
{
	uint32_t count = 0;
	TRY
	sql << "SELECT count(uid) FROM station", into(count);
	CATCH_MSG("[StorageDB] countStation(): ")
	return count;
}

uint64_t Storage::CountStationMeasurements(string uid)
{
	uint64_t count = 0;
	TRY
	sql << "SELECT count(*) FROM measurement "
		"WHERE station_uid = :uid", use(uid), into(count);
	CATCH_MSG("[db/Storage] countMeasurementFromStation() ")
	return count;
}

vector<entity::Station> Storage::GetStations()
{
	auto stations = vector<entity::Station>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM station");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		entity::Station element;
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

vector<entity::Metrics> Storage::GetMetrics()
{
	auto metrics = vector<entity::Metrics>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM metrics");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		entity::Metrics element;
		element.code = row.get<string>(0);
		element.equivalents = row.get<string>(1);
		element.type = row.get<string>(2);
		element.unit = row.get<string>(3);
		element.format = row.get<string>(4);
		metrics.push_back(element);
	}
	CATCH_MSG("[Storage] Exception at GetMetrics():\n\t")
	return metrics;
}

// TODO: move to DBAggregate?
vector<aggregation::entity::Task> Storage::GetTasks()
{
	auto tasks = vector<aggregation::entity::Task>();
	TRY
	rowset<row> rs = (sql.prepare << "SELECT * FROM aggregate_task");
	for (auto it = rs.begin(); it != rs.end(); ++it) {
		row const& row = *it;
		aggregation::entity::Task element;
		element.id = row.get<int>(0);
		element.period_name = row.get<string>(1);
		element.station_uid = row.get<string>(2);		
		element.current_ts = row.get<tm>(3);
		tasks.push_back(element);
	}
	CATCH_MSG("[Storage] Exception at GetTasks():\n\t")
	return tasks;
}

vector<aggregation::entity::Period> Storage::GetPeriods()
{
	auto periods = vector<aggregation::entity::Period>();
	TRY
  rowset<row> rs = (sql.prepare << "SELECT name FROM aggregate_period");
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    const row& row = *it;
    aggregation::entity::Period element;
    element.name = row.get<string>(0);   
    periods.push_back(element);
  }	
	CATCH_MSG("[Storage] Exception at GetPeriods():\n\t")
	return periods;
}

bool Storage::DeleteTask(uint16_t id)
{
	TRY
	sql << "DELETE FROM aggregate_task WHERE id = :id", use(id);
	return true;
	CATCH_MSG("[Storage] Exception at DeleteTask():\n\t")
	return false;
}

bool Storage::DeleteTask(string period_name, string station_uid)
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

map<time_t, string> Storage::GetMeasurement(
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

map<time_t, string> Storage::GetMeasurementFromLast(
  string station_uid, string metrics_code, uint16_t from_last_hours )
{
  time_t time_data = time(nullptr);
  tm to_time = *localtime(&time_data);
  time_data -= from_last_hours * 3600;
  tm from_time = *localtime(&time_data);
  return GetMeasurement(station_uid, metrics_code, from_time, to_time);
}

vector<string> Storage::GetMetricsCodes()
{
  static auto metrics = GetMetrics();
  auto result = vector<string>(metrics.size());
  for (auto it = metrics.begin(); it != metrics.end(); ++it) {
    result.push_back(it->code);
  }
  return result;
}

vector<string> Storage::GetStationMeasure(string station_uid, 
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

tm Storage::GetOldestStationMeasureTime(string uid)
{
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM "
    "(SELECT min(timestamp) FROM measurement WHERE station_uid = :uid))",
    use(uid), into(time);
  CATCH_MSG("[Storage] GetOldestStationMeasureTime():\n\t")
  return *gmtime(&time);
}

bool Storage::UpdateTaskCurrentTime(uint32_t id, tm timestamp)
{
  TRY
  sql << "UPDATE aggregate_task SET current_ts = :timestamp "
    "WHERE id = :id", use(timestamp), use(id);
  return true;
  CATCH_MSG("[Storage] UpdateTaskCurrentTime():\n\t")
  return false;
}

tm Storage::CalculateAggregateEndTime(string period_name, tm start_time)
{  
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM (SELECT (:start_time) + (SELECT period FROM aggregate_period "
    "WHERE name = :period_name)))", use(start_time),
    use(period_name), into(time);
  CATCH_MSG("[Storage] CalculateAggregateEndTime():\n\t")
  return *gmtime(&time);
}

bool Storage::UpdateStationLastUpdate(string station_uid, tm timestamp)
{
  TRY
  sql << "UPDATE station SET last_update = :last_update "
    "WHERE uid = :uid", use(timestamp), use(station_uid);
  return true;
  CATCH_MSG("[Storage] UpdateStationLastUpdate(station_uid, timestamp):\n\t")
  return false;
}

tm Storage::GetStationLastUpdate(string station_uid)
{
  time_t time = 0;
  TRY
  sql << "SELECT EXTRACT(EPOCH FROM (SELECT last_update FROM station "
    "WHERE uid = :uid))", use(station_uid), into(time);
  CATCH_MSG("[Storage] Exception at GetStationLastDataUpdate(station_uid):\n\t")
  return *gmtime(&time);
}

bool Storage::CreateTask(string period_name, string station_uid)
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
// ~~ stormy::db::Storage
}}
