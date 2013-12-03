#include "InitialAggregation.h"

#include <ctime>
#include <string>
#include <Poco/NumberFormatter.h>

using std::string;
using std::tm;
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
  logger_.information(prepareHeader("InitialAggregation") + 
    "] Has died.");
}

void InitialAggregation::run()
{
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information(prepareHeader("InitialAggregation") + 
    "] Running. Aggregated period [" + current_ts + " - ...]");

  // check if any measurements exists
  if(storage_->CountStationMeasures(task_entity_.station_uid) > 0) {
    // find oldest measure for station_uid
    tm oldest_measure = storage_->
      GetOldestStationMeasureTime(task_entity_.station_uid);

    logger_.information(prepareHeader("InitialAggregation") +
      "Oldest measure from " + asctime(&oldest_measure));

    time_t current_task_t = mktime(&task_entity_.current_ts);
    time_t oldest_measure_t = mktime(&oldest_measure);

    if (current_task_t < oldest_measure_t) {
      storage_->UpdateTaskCurrentTime(task_entity_.id, oldest_measure);
      task_entity_.current_ts = oldest_measure;
    }
  }  

  if (inner_scheduler_) {
    inner_scheduler_->ScheduleAsRegularTask(task_entity_);
  }
  else {
    logger_.warning("Inner scheduler is not set. "
      "Couldn't create RegularAggregation task.");
  }

  logger_.information(prepareHeader("InitialAggregation") + 
    "] Task ended.");  
}
// ~~ stormy::aggregate::task::InitialAggregation
}}}