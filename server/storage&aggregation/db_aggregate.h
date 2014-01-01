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
  Aggregate(common::db::Setting aggregate_setting, Storage* database_storage);
  ~Aggregate();
    
  // tasks
  std::tm GetTaskCurrentTS(uint32_t task_id);
  std::string GetStationUIDFromTask(uint32_t task_id);

  // aggregate
  bool InsertAggregate(stormy::aggregation::entity::Aggregate aggregate);

  Storage* database_storage() {
    return database_storage_;
  }
  common::db::Setting Configuration() {
    return aggregate_setting_;
  }
private:  
  void Connect();

  common::db::Setting aggregate_setting_;
  Storage* database_storage_;
  soci::session sql;
  Poco::Logger& logger_;
};
// ~~ stormy::db::Aggregate
}}
