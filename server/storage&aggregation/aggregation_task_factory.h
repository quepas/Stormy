#pragma once

#include "../../common/db_setting.h"
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
  Factory(
    common::db::Setting storage_setting, 
    common::db::Setting aggregate_setting);
  ~Factory() {};

  Base* CreateDynamicTask(TaskType task_type, entity::Task task_entity);
  void SetInnerScheduler(Scheduler* inner_scheduler);

private:
  common::db::Setting storage_setting_;
  common::db::Setting aggregate_setting_;  
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Factory
}}}
