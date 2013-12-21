#pragma once

#include "DBStorage.h"
#include "DBAggregation.h"
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
  Factory(Stormy::DBStorage* database_storage, Stormy::DBAggregation* database_aggregation);
  ~Factory() {};

  Base* CreateDynamicTask(TaskType task_type, entity::Task task_entity);
  void SetInnerScheduler(Scheduler* inner_scheduler);

private:
  Stormy::DBStorage* database_storage_;
  Stormy::DBAggregation* database_aggregation_;
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Factory
}}}
