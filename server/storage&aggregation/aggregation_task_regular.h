#pragma once

#include "aggregation_task_base.h"
#include "DBStorage.h"
#include "DBAggregation.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Regular : public Base
{
public:
  Regular(
    entity::Task task_data, 
    Stormy::DBStorage* storage, 
    Stormy::DBAggregation* aggregation);
  ~Regular();

  void run() override;
};
// ~~ stormy::aggregation::task::Regular
}}}