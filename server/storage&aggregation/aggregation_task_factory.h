#pragma once

#include "db_storage.h"
#include "db_aggregate.h"
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
  Factory() {};
  Factory(db::Storage* database_storage, db::Aggregate* database_aggregation);
  ~Factory() {};

  Base* CreateDynamicTask(TaskType task_type, entity::Task task_entity);
  void SetInnerScheduler(Scheduler* inner_scheduler);

private:
  db::Storage* database_storage_;
  db::Aggregate* database_aggregation_;
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Factory
}}}
