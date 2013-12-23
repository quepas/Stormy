#pragma once

#include "aggregation_task_base.h"
#include "db_storage.h"
#include "db_aggregate.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Regular : public Base
{
public:
  Regular(
    entity::Task task_data, 
    db::Storage* storage, 
    db::Aggregate* aggregation);
  ~Regular();

  void run() override;
};
// ~~ stormy::aggregation::task::Regular
}}}