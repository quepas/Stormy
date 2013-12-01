#pragma once

#include "BaseTask.h"
#include "../Scheduler.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class InitialAggregation : public BaseTask
{
public:
  InitialAggregation(entity::Task task_data, Scheduler* scheduler);
  ~InitialAggregation();

  void run() override;
private:
  Scheduler* scheduler_;
};
// ~~ stormy::aggregate::task::InitialAggregation
}}}