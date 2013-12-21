#pragma once

#include "aggregation_task_base.h"
#include "aggregation_scheduler.h"

#include "DBStorage.h"
#include "DBAggregation.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Initial : public Base
{
public:
  Initial(entity::Task task_data, Stormy::DBStorage* storage, 
    Stormy::DBAggregation* aggregation, Scheduler* inner_scheduler);
  ~Initial();

  void run() override;  
private:
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Initial
}}}