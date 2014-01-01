#pragma once

#include "../../common/db_setting.h"
#include "aggregation_task_base.h"
#include "aggregation_scheduler.h"
#include "db_storage.h"
#include "db_aggregate.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Initial : public Base
{
public:
  Initial(
    entity::Task task_data, 
    common::db::Setting storage_setting, 
    common::db::Setting aggregate_setting, 
    Scheduler* inner_scheduler);
  ~Initial();

  void run() override;  
private:
  Scheduler* inner_scheduler_;
};
// ~~ stormy::aggregation::task::Initial
}}}