#include "db_aggregate.h"

#include "../../common/util.h"

#include <postgresql/soci-postgresql.h>

using namespace stormy::aggregation;
using std::tm;
using std::time_t;
using std::string;
using std::vector;
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
// ~~ stormy::db::Aggregate
}}
