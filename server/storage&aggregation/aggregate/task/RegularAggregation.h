#pragma once

#include "Base.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class RegularAggregation : public Base
{
public:
  explicit RegularAggregation(entity::Task task_data);
  ~RegularAggregation();

  void run() override;
};
// ~~ stormy::aggregate::task::RegularAggregation
}}}