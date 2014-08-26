#include "db_storage.h"

#include <exception>
#include <iostream>

#include <boost/range/algorithm/copy.hpp>
#include <postgresql/soci-postgresql.h>

using namespace stormy::common;

using boost::copy;
using std::back_inserter;
using std::map;
using std::make_pair;
using std::string;
using std::tm;
using std::time_t;
using std::vector;
using Poco::Logger;
using soci::postgresql;
using soci::use;
using soci::into;
using soci::row;
using soci::rowset;
using soci::i_null;

namespace stormy {
  namespace db {

Storage::Storage(DatabaseSetting setting)
  : logger_(Logger::get("aggregation_main_thread")),
    setting_(setting)
{
  connect();
}

Storage::~Storage()
{

}

void Storage::connect()
{
  TRY
  sql_.open(postgresql, ToPostgreString(setting_));
  CATCH_MSG("[db/Storage] connect: ")
}

void Storage::InsertStation(entity::Station station)
{
  TRY
  sql_ << "INSERT INTO station(uid, name, url, refresh_time, last_update)"
    "values(:uid, :name, :url, :refresh_time, to_timestamp(0))",
    use(station.uid), use(station.name), 
    use(station.url), use(station.refresh_time);
  CATCH_MSG("[db/Storage] insertStation: ")
}

void Storage::InsertStations(const vector<entity::Station>& stations)
{
	Each(stations, [&](entity::Station station) {
		if(!CheckIfStationExsist(station.uid))
			InsertStation(station);
	});
}

entity::Station Storage::GetStationByUID(string uid)
{
	entity::Station station;
	TRY
	sql_ << "SELECT uid, name, url, refresh_time FROM station WHERE uid = :uid",
		into(station.uid), into(station.name), into(station.url),
		into(station.refresh_time), use(uid);
	CATCH_MSG("[db/Storage] GetStationByUID: ")
	return station;
}

bool Storage::CheckIfStationExsist(string uid)
{	
  int count = 0;
	TRY
  sql_ << "SELECT count(*) FROM station WHERE uid = :uid",
    into(count), use(uid);
	CATCH_MSG("[db/Storage] existsStationByUID: ")	
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
    sql_ << "UPDATE station SET last_update = :last_update "
      "WHERE uid = :uid", use(last_update), use(station_uid);
    CATCH_MSG("[db/Storage] UpdateStationLastUpdateIfNeeded: ")
  }
}


void Storage::InsertMeasureAsText(const entity::Measurement& measure)
{
  TRY  
  sql_ << "INSERT INTO measurement"
    "(metrics_code, value_text, value_number, station_uid, timestamp)"
    "values(:metrics_code, :value_text, NULL, :station_uid, :timestamp)", 
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
  sql_ << "INSERT INTO measurement"
    "(metrics_code, value_number, value_text, station_uid, timestamp)"
    "values(:metrics_code, :value_number, NULL, :station_uid, :timestamp)", 
    use(measure.code),
    use(measure.value_number),
    use(measure.station_uid),
    use(measure.timestamp);
  CATCH_MSG("[db/Storage] InsertMeasureAsText: ")
  UpdateStationLastUpdateIfNeeded(measure.station_uid, measure.timestamp);
}

tm Storage::GetNewestStationMeasureTime(string station_uid)
{	
  tm newest_measure_time;
	if(CountStationMeasurements(station_uid) > 0) {
		TRY
    sql_ << "SELECT max(timestamp) FROM measurement "
      "WHERE station_uid = :station_uid",
      into(newest_measure_time), use(station_uid);
		CATCH_MSG("[db/Storage] findNewestMeasureTimeByStationUID: ")
	}
	return newest_measure_time;
}

bool Storage::InsertSingleMetrics(entity::Metrics metrics)
{
  TRY
  sql_ << "INSERT INTO metrics(code, equivalents, type, unit, format)"
    "values(:code, :equivalents, :type, :unit, :format)",
    use(metrics.code), use(metrics.equivalents),
    use(metrics.type), use(metrics.unit),
    use(metrics.format);
  return true;
  CATCH_MSG("[db/Storage] insertOneMetrics: ")
	return false;	
}

bool Storage::InsertMetrics(const vector<entity::Metrics>& metrics)
{
	if(!metrics.empty()) {
		Each(metrics, [&](entity::Metrics metric) {
			if(!CheckIfMetricsExsist(metric.code))
				InsertSingleMetrics(metric);
		});
	}
	return true;
}

bool Storage::CheckIfMetricsExsist(const string& code)
{
	uint32_t count = 0;
	TRY
	sql_ << "SELECT count(*) FROM metrics WHERE code = :code",
		into(count), use(code);
	CATCH_MSG("[db/Storage] existsMetricsByCode: ")
	return count > 0;
}

uint64_t Storage::CountAllMeasurements()
{
	uint64_t count = 0;
	TRY
	sql_ << "SELECT count(*) FROM measurement", into(count);
	CATCH_MSG("[db/Storage] CountAllMeasurements: ")
	return count;
}

uint32_t Storage::CountAllStations()
{
	uint32_t count = 0;
	TRY
	sql_ << "SELECT count(uid) FROM station", into(count);
	CATCH_MSG("[db/Storage] CountAllStations: ")
	return count;
}

uint64_t Storage::CountStationMeasurements(string uid)
{
	uint64_t count = 0;
	TRY
	sql_ << "SELECT count(*) FROM measurement "
		"WHERE station_uid = :uid", use(uid), into(count);
	CATCH_MSG("[db/Storage] CountStationMeasurements: ")
	return count;
}

vector<entity::Station> Storage::GetStations()
{
	auto stations = vector<entity::Station>();
	TRY
	rowset<row> rs = (sql_.prepare << "SELECT * FROM station");
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
	CATCH_MSG("[db/Storage] GetStations: ")
	return stations;
}

vector<entity::Metrics> Storage::GetMetrics()
{
	auto metrics = vector<entity::Metrics>();
	TRY
	rowset<row> rs = (sql_.prepare << "SELECT * FROM metrics");
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
	CATCH_MSG("[db/Storage] GetMetrics: ")
	return metrics;
}

vector<string> Storage::GetMetricsCodes()
{
  static auto metrics = GetMetrics();
  auto result = vector<string>();
  for (auto it = metrics.begin(); it != metrics.end(); ++it) {
    if (!it->code.empty())
      result.push_back(it->code);
  }
  return result;
}

tm Storage::GetOldestStationMeasureTime(string uid)
{
  tm measure_oldest_time;
  TRY
  sql_ << "SELECT min(timestamp) FROM measurement WHERE station_uid = :uid",
    use(uid), into(measure_oldest_time);
  CATCH_MSG("[db/Storage] GetOldestStationMeasureTime: ")
  return measure_oldest_time;
}

tm Storage::GetStationLastUpdate(string station_uid)
{
  tm station_last_update;
  TRY
  sql_ << "SELECT last_update FROM station WHERE uid = :uid", 
    use(station_uid), into(station_last_update);
  CATCH_MSG("[db/Storage] GetStationLastDataUpdate: ")
  return station_last_update;
}

vector<tm> Storage::SelectDistinctMeasureTSForStationBetweenTS(
  string station_uid,
  tm from,
  tm to)
{
  vector<tm> distinct_timestamps;
  TRY
  rowset<row> rs = (sql_.prepare <<
    "SELECT DISTINCT timestamp "
      "FROM measurement "
      "WHERE station_uid = :station_uid "
      "AND timestamp >= :from_ts "
      "AND timestamp <= :to_ts "
      "ORDER BY timestamp ",
    use(station_uid), use(from), use(to));

  for (auto row = rs.begin(); row != rs.end(); ++row) {    
    distinct_timestamps.push_back(row->get<tm>(0));
  }
  CATCH_MSG("[db/Storage] SelectDistinctMeasureTSForStationBetweenTS: ")
  return distinct_timestamps;
}

map<time_t, vector<entity::Measurement>> 
  Storage::GetMeasureSetsForStationBetweenTS(
    string station_uid, 
    time_t from, 
    time_t to)
{    
  map<time_t, vector<entity::Measurement>> measure_sets;  
  tm from_time = *localtime(&from);
  tm to_time = *localtime(&to);

  vector<tm> measure_set_times = 
    SelectDistinctMeasureTSForStationBetweenTS(station_uid, from_time, to_time);
  for (auto mst_it = measure_set_times.begin(); 
        mst_it != measure_set_times.end(); ++mst_it) {
    vector<entity::Measurement> measures;
    TRY
    rowset<row> rs = (sql_.prepare << 
      "SELECT * FROM measurement "
        "WHERE station_uid = :station_uid "
        "AND timestamp = :ms_time ", 
      use(station_uid), use(*mst_it));
    
    for (auto row = rs.begin(); row != rs.end(); ++row) {    
      entity::Measurement measure;
      measure.id = row->get<int>(0);
      measure.code = row->get<string>(1);
      measure.station_uid = row->get<string>(2);

      const auto& value_text_ind = row->get_indicator(3);
      if (value_text_ind != i_null) {        
        measure.value_text = row->get<string>(3);
        measure.is_numeric = false;
      }
      const auto& value_number_ind = row->get_indicator(4);
      if (value_number_ind != i_null) {       
        measure.value_number = row->get<double>(4);
        measure.is_numeric = true;
      }
      measure.timestamp = row->get<tm>(5);
      measures.push_back(measure);
    }
    CATCH_MSG("[db/Storage] GetMeasureSetsForStationBetweenTS: ")
    time_t measure_set_time = mktime(&(*mst_it))+3600;
    measure_sets.insert(make_pair(measure_set_time, measures));
  }
  return measure_sets;
}

map<time_t, vector<entity::Measurement>> 
  Storage::GetMeasureSetsForStationAndTS(string station_uid, time_t time )
{
  return GetMeasureSetsForStationBetweenTS(station_uid, time, time);
}

map<time_t, vector<entity::Measurement>> 
  Storage::GetAllMeasureSetsForStation(string station_uid)
{
  return GetMeasureSetsForStationBetweenTS(station_uid, 0, LocaltimeNow());
}
// ~~ stormy::db::Storage
}}
