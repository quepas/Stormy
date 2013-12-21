#pragma once

#include "Base.h"
#include "../Scheduler.h"

#include "../../DBStorage.h"
#include "../../DBAggregation.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class InitialAggregation : public Base
{
public:
  InitialAggregation(aggregation::entity::Task task_data, Stormy::DBStorage* storage, 
    Stormy::DBAggregation* aggregation, Scheduler* inner_scheduler);
  ~InitialAggregation();

  void run() override;  
private:
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregate::task::InitialAggregation
}}}