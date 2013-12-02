#pragma once

#include "Base.h"
#include "../Scheduler.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class InitialAggregation : public Base
{
public:
  InitialAggregation(entity::Task task_data, Scheduler* inner_scheduler);
  ~InitialAggregation();

  void run() override;  
private:
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregate::task::InitialAggregation
}}}