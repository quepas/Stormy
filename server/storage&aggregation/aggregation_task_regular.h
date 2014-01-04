#pragma once

#include "db_storage.h"
#include "db_aggregate.h"
#include "aggregation_task_base.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Regular : public Base
{
public:
  Regular(
    entity::Task task_data, 
    db::Storage& storage_database,
    db::Aggregate& aggregation_database);
  ~Regular();

  void run() override;
};
// ~~ stormy::aggregation::task::Regular
}}}