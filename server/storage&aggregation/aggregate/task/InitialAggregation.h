#pragma once

#include "BaseTask.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class InitialAggregation : public BaseTask
{
public:
  InitialAggregation(entity::Task task_data);
  ~InitialAggregation();

  void run();
private:
};
// ~~ stormy::aggregate::task::InitialAggregation
}}}