#pragma once

#include "../../common/db_setting.h"
#include "aggregation_task_base.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Regular : public Base
{
public:
  Regular(
    entity::Task task_data, 
    common::db::Setting storage_setting, 
    common::db::Setting aggregate_setting);
  ~Regular();

  void run() override;
};
// ~~ stormy::aggregation::task::Regular
}}}