#pragma once

#include "../../DBStorage.h"
#include "../../DBAggregation.h"
#include "../../aggregation_entity_task.h"
#include "Base.h"

namespace stormy {
  namespace aggregate {   
      class Scheduler;  // forward declaration 
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

  Base* createDynamicTask(TaskType task_type, aggregation::entity::Task task_entity);
  void SetInnerScheduler(Scheduler* inner_scheduler);

private:
  Stormy::DBStorage* database_storage_;
  Stormy::DBAggregation* database_aggregation_;
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregate::task::Factory
}}}
