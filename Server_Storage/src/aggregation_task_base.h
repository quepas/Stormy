#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "aggregation_entity_task.h"
#include "db_storage.h"
#include "db_aggregate.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Base : public Poco::Util::TimerTask
{
public:
  Base(
    entity::Task task_data,
    db::Storage& storage_database,
    db::Aggregate& aggregation_database);
  virtual ~Base();

  void run() = 0;
protected:
  entity::Task task_entity_;
  db::Storage& storage_database_;
  db::Aggregate& aggregation_database_;
  Poco::Logger& logger_;

  std::string PrepareHeader(std::string task_type);
};
// ~~ stormy::aggregation::task::Base
}}}