#include "aggregation_task_factory.h"

#include "aggregation_task_initial.h"
#include "aggregation_task_regular.h"

namespace stormy {
  namespace aggregation {
    namespace task {

Factory::Factory(
  db::Storage* database_storage, 
  db::Aggregate* database_aggregation) 
  : database_storage_(database_storage),
    database_aggregation_(database_aggregation),
    inner_scheduler_(nullptr)
{

}

Base* Factory::CreateDynamicTask(TaskType task_type, entity::Task task_entity)
{
    switch (task_type) {
      case INITIAL: {
        return new Initial(task_entity, 
          database_storage_, database_aggregation_, inner_scheduler_);        
      }
      case REGULAR: {
        return new Regular(task_entity, 
          database_storage_, database_aggregation_);
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
