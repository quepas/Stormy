#include "db_aggregate.h"

#include "../../common/util.h"

#include <postgresql/soci-postgresql.h>

using namespace stormy::aggregation;
using std::tm;
using std::time_t;
using std::string;
using std::vector;
using std::map;
using Poco::Logger;
using soci::postgresql;
using soci::use;
using soci::into;
using soci::row;
using soci::rowset;

namespace stormy {
  namespace db {

Aggregate::Aggregate(common::db::Setting aggregate_setting)
	:	logger_(Logger::get("aggregation")),
    aggregate_setting_(aggregate_setting)
{
	Connect();
}

Aggregate::~Aggregate()
{

}

void Aggregate::Connect()
{
	TRY
	sql_.open(postgresql, aggregate_setting_.AsConnectionString());
	CATCH_MSG("[db/Aggregate] connect(): ")
}

string Aggregate::GetStationUIDFromTask(uint32_t task_id)
{
	string station_uid = "";
	TRY
	sql_ << "SELECT station_uid FROM aggregate_task "
		"WHERE id = :id", use(task_id), into(station_uid);
	CATCH_MSG("[db/Aggregate] getStationIdFromTask(): ")
	return station_uid;
}

tm Aggregate::GetTaskCurrentTS(uint32_t task_id)
{
  tm timestamp;
  TRY
  sql_ << "SELECT current_ts FROM aggregate_task "
    "WHERE id = :id", use(task_id), into(timestamp);
  CATCH_MSG("[db/Aggregate] GetTaskCurrentTS():\n\t")
  return timestamp;
}

bool Aggregate::InsertAggregate(entity::Aggregate aggregate)
{
  TRY
  sql_ << "INSERT INTO aggregate (station_uid, metrics_code, "
    "operation_name, period_name, start_time, value, sample_number) "
    "VALUES(:station_uid, "
    ":metrics_code, :operation_name, :period_name, "
    ":start_time, :value, :sample_number)",
    use(aggregate.station_uid), use(aggregate.metrics_code),
    use(aggregate.operation_name), use(aggregate.period_name),
    use(aggregate.start_time), use(aggregate.value), 
    use(aggregate.sample_number);
  return true;
  CATCH_MSG("[db/Aggregate] InsertAggregate():\n\t")
  return false;
}

vector<entity::Task> Aggregate::GetTasks()
{
  auto tasks = vector<entity::Task>();
  TRY
  rowset<row> rs = (sql_.prepare << "SELECT * FROM aggregate_task");
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    row const& row = *it;
    entity::Task element;
    element.id = row.get<int>(0);
    element.period_name = row.get<string>(1);
    element.station_uid = row.get<string>(2);		
    element.current_ts = row.get<tm>(3);
    tasks.push_back(element);
  }
  CATCH_MSG("[db/Aggregate] GetTasks: ")
  return tasks;
}

vector<entity::Period> Aggregate::GetPeriods()
{
  auto periods = vector<entity::Period>();
  TRY
  rowset<row> rs = (sql_.prepare << "SELECT name FROM aggregate_period");
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    const row& row = *it;
    entity::Period element;
    element.name = row.get<string>(0);   
    periods.push_back(element);
  }	
  CATCH_MSG("[db/Aggregate] GetPeriods: ")
  return periods;
}

vector<entity::Operation> Aggregate::GetOperations()
{
  auto operations = vector<entity::Operation>();
  TRY
  rowset<row> rs = (sql_.prepare << "SELECT * FROM aggregate_operation");
  for (auto it = rs.begin(); it != rs.end(); ++it) {
    const row& row = *it;
    entity::Operation element;
    element.name = row.get<string>(0);
    element.analysis_method = row.get<string>(1);
    operations.push_back(element);
  }	
  CATCH_MSG("[db/Aggregate] GetOperations: ")
  return operations;
};

bool Aggregate::CreateTask(string period_name, string station_uid)
{
  TRY
  sql_ << "INSERT INTO aggregate_task "
    "(period_name, station_uid, current_ts)"
    "VALUES(:period_name, :station_uid, to_timestamp(0))",
    use(period_name), use(station_uid);
  return true;
  CATCH_MSG("[db/Storage] CreateTask: ")
  return false;
}

bool Aggregate::DeleteTask(string period_name, string station_uid)
{
  TRY
  sql_ << "DELETE FROM aggregate_task "
    "WHERE period_name = :period_name "
    "AND station_uid = :station_uid",
    use(period_name), use(station_uid);
  return true;
  CATCH_MSG("[db/Aggregate] DeleteTask: ")
  return false;
}

bool Aggregate::UpdateTaskCurrentTime(uint32_t id, tm timestamp)
{
  TRY
  sql_ << "UPDATE aggregate_task SET current_ts = :timestamp "
    "WHERE id = :id", use(timestamp), use(id);
  return true;
  CATCH_MSG("[db/Aggregate] UpdateTaskCurrentTime: ")
  return false;
}

tm Aggregate::CalculateAggregateEndTime(string period_name, tm start_time)
{    
  tm aggregate_end_time;
  time_t start_time_t = mktime(&start_time);
  TRY
  sql_ << "SELECT to_timestamp(:start_time) + "
    "(SELECT period FROM aggregate_period "
    " WHERE name = :period_name)::interval",
   use(start_time_t), use(period_name), into(aggregate_end_time);  
  CATCH_MSG("[db/Aggregate] CalculateAggregateEndTime: ")
  return aggregate_end_time;
}

uint64_t Aggregate::CountStationAggregates(string station_uid)
{
  uint64_t count = 0;
  TRY
  sql_ << "SELECT count(*) FROM aggregate "
    "WHERE station_uid = :station_uid", 
    use(station_uid), into(count);
  CATCH_MSG("[db/Aggregate] CountStationAggregate: ")
  return count;
}

uint64_t Aggregate::CountStationPeriodAggregates(
  string station_uid, 
  string period_name)
{
  uint64_t count = 0;
  TRY
  sql_ << "SELECT count(*) FROM aggregate "
    "WHERE station_uid = :station_uid "
    "AND period_name = :period_name",
    use(station_uid), use(period_name), into(count);
  CATCH_MSG("[db/Aggregate] CountStationPeriodAggregates: ")
  return count;
}

vector<tm> Aggregate::SelectAllDistinctAggregateTSForStationPeriod(
  string station_uid, 
  string period_name)
{
  time_t from = 0;
  time_t to = common::LocaltimeNow();

  return SelectDistinctAggregateTSForStationPeriodBetweenTS(
    station_uid,
    period_name,
    *localtime(&from), 
    *localtime(&to));
}

vector<tm> Aggregate::SelectDistinctAggregateTSForStationPeriodBetweenTS(
  string station_uid,
  string period_name, 
  tm from, 
  tm to)
{
  vector<tm> distinct_timestamp;
  TRY
  rowset<row> rs = (sql_.prepare <<
    "SELECT DISTINCT start_time "
      "FROM aggregate "
      "WHERE station_uid = :station_uid "
      "AND period_name = :period_name " 
      "AND start_time >= :from_ts "
      "AND start_time <= :to_ts "
      "ORDER BY start_time ",
    use(station_uid), use(period_name), use(from), use(to));

  for (auto row = rs.begin(); row != rs.end(); ++row) {    
    distinct_timestamp.push_back(row->get<tm>(0));
  }
  CATCH_MSG("[db/Aggregate] SelectDistinctAggregateTSForStationBetweenTS: ")
  return distinct_timestamp;
}

map<time_t, vector<aggregation::entity::Aggregate>> Aggregate::
  GetAggregateSetsForStationPeriodBetweenTS(
    string station_uid, 
    string period_name, 
    time_t from, 
    time_t to)
{
  map<time_t, vector<aggregation::entity::Aggregate>> aggregate_sets;
  tm from_time = *localtime(&from);
  tm to_time = *localtime(&to);

  auto aggregate_set_times =
    SelectDistinctAggregateTSForStationPeriodBetweenTS(
      station_uid, 
      period_name, 
      from_time, 
      to_time);

  for (auto ast_it = aggregate_set_times.begin();
        ast_it != aggregate_set_times.end(); ++ast_it) {
    vector<aggregation::entity::Aggregate> aggregates;
    TRY
    rowset<row> rs = (sql_.prepare << 
      "SELECT * FROM aggregate "
        "WHERE station_uid = :station_uid "
        "AND period_name = :period_name "
        "AND start_time = :ms_time ", 
      use(station_uid), use(period_name), use(*ast_it));

    for (auto row = rs.begin(); row != rs.end(); ++row) {
      aggregation::entity::Aggregate aggregate;
      aggregate.id = row->get<int>(0);
      aggregate.station_uid = row->get<string>(1);
      aggregate.metrics_code = row->get<string>(2);
      aggregate.operation_name = row->get<string>(3);
      aggregate.period_name = row->get<string>(4);
      aggregate.start_time = row->get<tm>(5);
      aggregate.value = row->get<double>(6);
      aggregate.sample_number = row->get<int>(7);
      aggregates.push_back(aggregate);
    }
    CATCH_MSG("[db/Aggregate] GetAggregateSetsForStationPeriodBetweenTS: ")
    time_t aggregate_set_time = mktime(&(*ast_it))+3600;  // make local!
    aggregate_sets.insert(make_pair(aggregate_set_time, aggregates));
  }
  return aggregate_sets;
}

map<time_t, vector<aggregation::entity::Aggregate>> Aggregate::
  GetAggregateSetsForStationPeriodAndTS(
    string station_uid, 
    string period_name, 
    time_t time)
{
  return GetAggregateSetsForStationPeriodBetweenTS(
    station_uid, period_name, time, time);
}
// ~~ stormy::db::Aggregate
}}
