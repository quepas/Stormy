#include "aggregation_task_factory.h"

#include "aggregation_task_initial.h"
#include "aggregation_task_regular.h"

namespace stormy {
  namespace aggregation {
    namespace task {

Factory::Factory(
  db::Storage& storage_database,
  db::Aggregate& aggregation_database) 
  : storage_database_(storage_database),
    aggregation_database_(aggregation_database),
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
          storage_database_, 
          aggregation_database_, 
          inner_scheduler_);        
      }
      case REGULAR: {
        return new Regular(
          task_entity, 
          storage_database_, 
          aggregation_database_);
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
