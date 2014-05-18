#pragma once

#include "db_aggregate.h"
#include "db_storage.h"
#include "aggregation_entity_task.h"
#include "aggregation_task_base.h"

namespace stormy {
  namespace aggregation {   
      class Scheduler;
        namespace task {

enum TaskType {
  INITIAL,
  REGULAR
};

class Factory
{
public:  
  Factory(
    db::Storage& storage_database,
    db::Aggregate& aggregation_database);
  ~Factory() {};

  Base* CreateDynamicTask(TaskType task_type, entity::Task task_entity);
  void SetInnerScheduler(Scheduler* inner_scheduler);

private:
  db::Storage& storage_database_;
  db::Aggregate& aggregation_database_;
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Factory
}}}
