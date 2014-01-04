#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <cstdint>
#include <soci.h>

#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "aggregation_entity_task.h"
#include "aggregation_entity_period.h"
#include "aggregation_entity_aggregate.h"

namespace stormy {
  namespace db {

class Aggregate
{
public:
  Aggregate(common::db::Setting aggregate_setting);
  ~Aggregate();
    
  // tasks
  std::tm GetTaskCurrentTS(uint32_t task_id);
  std::string GetStationUIDFromTask(uint32_t task_id);

  bool DeleteTask(std::string period_name, std::string station_uid);
  bool CreateTask(std::string period_name, std::string station_uid);
  bool UpdateTaskCurrentTime(uint32_t id, std::tm timestamp);
  std::tm CalculateAggregateEndTime(std::string period_name, std::tm start_time);  
  std::vector<aggregation::entity::Task> GetTasks();
  std::vector<aggregation::entity::Period> GetPeriods();   
  // aggregate
  bool InsertAggregate(aggregation::entity::Aggregate aggregate);
  
  common::db::Setting Configuration() {
    return aggregate_setting_;
  }
private:  
  void Connect();

  common::db::Setting aggregate_setting_;  
  soci::session sql_;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Aggregate
}}
