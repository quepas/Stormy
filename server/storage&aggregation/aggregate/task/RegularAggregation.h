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
};
// ~~ stormy::aggregate::task::RegularAggregation
}}}