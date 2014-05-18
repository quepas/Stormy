#pragma once

#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <soci.h>

#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "aggregation_entity_task.h"
#include "aggregation_entity_period.h"
#include "aggregation_entity_aggregate.h"
#include "aggregation_entity_operation.h"

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

  bool IfOperationExsist(std::string operation_name);
  bool InsertOperation(const aggregation::entity::Operation& operation);
  std::vector<aggregation::entity::Operation> GetOperations();

  // aggregate
  bool InsertAggregate(aggregation::entity::Aggregate aggregate);
  uint64_t CountStationAggregates(std::string station_uid);
  uint64_t CountStationPeriodAggregates(
    std::string station_uid, 
    std::string period_name);

  std::vector<std::tm> SelectDistinctAggregateTSForStationPeriodBetweenTS(
    std::string station_uid, 
    std::string period_name,
    std::tm from, 
    std::tm to);

  std::vector<std::tm> SelectAllDistinctAggregateTSForStationPeriod(
    std::string station_uid, 
    std::string period_name);

  std::map<std::time_t, std::vector<aggregation::entity::Aggregate>> 
    GetAggregateSetsForStationPeriodBetweenTS(
      std::string station_uid, 
      std::string period_name,
      std::time_t from, 
      std::time_t to);
  std::map<std::time_t, std::vector<aggregation::entity::Aggregate>> 
    GetAggregateSetsForStationPeriodAndTS(
      std::string station_uid,
      std::string period_name,
      std::time_t time);
  
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
