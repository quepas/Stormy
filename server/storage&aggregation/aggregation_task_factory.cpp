#include "aggregation_task_factory.h"

#include "aggregation_task_initial.h"
#include "aggregation_task_regular.h"

namespace stormy {
  namespace aggregation {
    namespace task {

Factory::Factory(
  common::db::Setting storage_setting, 
  common::db::Setting aggregate_setting) 
  : storage_setting_(storage_setting),
    aggregate_setting_(aggregate_setting),
    inner_scheduler_(nullptr)
{

}

Base* Factory::CreateDynamicTask(
  TaskType task_type, 
  entity::Task task_entity)
{
    switch (task_type) {
      case INITIAL: {
        return new Initial(
          task_entity, 
          storage_setting_, 
          aggregate_setting_, 
          inner_scheduler_);        
      }
      case REGULAR: {
        return new Regular(
          task_entity, 
          storage_setting_, 
          aggregate_setting_);
      }      
    }
    return nullptr;
}

void Factory::SetInnerScheduler(Scheduler* inner_scheduler)
{
  inner_scheduler_ = inner_scheduler;
}
// ~~ stormy::aggregation::task::Factory
}}}
