#include "InitialAggregation.h"

#include <ctime>
#include <string>
#include <Poco/NumberFormatter.h>

using std::string;
using std::asctime;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregate {
    namespace task {

InitialAggregation::InitialAggregation(entity::Task task_data, 
  Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation, 
  Scheduler* inner_scheduler)
  : Base(task_data, storage, aggregation),
    inner_scheduler_(inner_scheduler)
{
}

InitialAggregation::~InitialAggregation()
{
  logger_.information("[aggregate::InitialAggregation#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Has died.");
}

void InitialAggregation::run()
{
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information("[aggregate::InitialAggregation#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Running. Aggregated period [" + current_ts + " - ...]");

  if (inner_scheduler_) {
    inner_scheduler_->ScheduleAsRegularTask(task_entity_);
  }
  else {
    logger_.warning("Inner scheduler is not set. "
      "Couldn't create RegularAggregation task.");
  }

  logger_.information("[aggregate::InitialAggregation#" + 
    NumberFormatter::format(task_entity_.id) + "] Task ended.");  
}
// ~~ stormy::aggregate::task::InitialAggregation
}}}