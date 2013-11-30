#pragma once

#include "BaseTask.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class RegularAggregation : public BaseTask
{
public:
  explicit RegularAggregation(entity::Task task_data);
  ~RegularAggregation();

  void run();
private:
};
// ~~ stormy::aggregate::task::RegularAggregation
}}}