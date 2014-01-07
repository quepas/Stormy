#pragma once

#include <vector>

#include "db_storage.h"
#include "db_aggregate.h"
#include "aggregation_task_base.h"
#include "aggregation_entity_operation.h"

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
private:
  std::vector<entity::Operation> available_operations_;
};
// ~~ stormy::aggregation::task::Regular
}}}