#pragma once

#include <string>
#include <cstdint>
#include <soci.h>

#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "db_storage.h"
#include "aggregation_setting.h"

#include "aggregation_entity_aggregate.h"

namespace stormy {
  namespace db {

class Aggregate
{
public:
  Aggregate(common::db::Setting* database_setting, Storage* database_storage);
  ~Aggregate();
    
  // tasks
  uint32_t GetTaskId(std::string station_uid, std::string period_name);
  std::tm GetTaskCurrentTS(uint32_t task_id);
  std::string GetStationUIDFromTask(uint32_t task_id);     

  // aggregate
  bool InsertAggregate(stormy::aggregation::entity::Aggregate aggregate);
																
  Storage* database_storage() {
    return database_storage_;
  }
private:  
  uint32_t InsertTask(std::string station_uid, stormy::aggregation::Setting aggregation_setting);
  void Connect();

  common::db::Setting* database_setting_;
  Storage* database_storage_;
  soci::session sql;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Aggregate
}}
