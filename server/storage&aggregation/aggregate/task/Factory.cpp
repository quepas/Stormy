#include "Factory.h"

#include "InitialAggregation.h"
#include "RegularAggregation.h"

namespace stormy {
  namespace aggregate {
    namespace task {

Factory::Factory( Stormy::DBStorage* database_storage, Stormy::DBAggregation* database_aggregation) 
  : database_storage_(database_storage),
    database_aggregation_(database_aggregation),
    inner_scheduler_(nullptr)
{

}

/*Factory::~Factory()
{

}*/

Base* Factory::createDynamicTask( TaskType task_type, aggregation::entity::Task task_entity )
{
    switch (task_type) {
      case INITIAL: {
        return new InitialAggregation(task_entity, 
          database_storage_, database_aggregation_, inner_scheduler_);        
      }
      case REGULAR: {
        return new RegularAggregation(task_entity, 
          database_storage_, database_aggregation_);
      }      
    }
    return nullptr;
}

void Factory::SetInnerScheduler( Scheduler* inner_scheduler )
{
  inner_scheduler_ = inner_scheduler;
}
// ~~ stormy::aggregate::task::Factory
}}}
