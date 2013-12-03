#pragma once

#include "Base.h"
#include "../../DBStorage.h"
#include "../../DBAggregation.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class RegularAggregation : public Base
{
public:
  RegularAggregation(entity::Task task_data, 
    Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation);
  ~RegularAggregation();

  void run() override;
};
// ~~ stormy::aggregate::task::RegularAggregation
}}}