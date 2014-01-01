#include "aggregation_task_initial.h"

#include <ctime>
#include <string>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <Poco/NumberFormatter.h>

using std::string;
using std::tm;
using std::asctime;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregation {
    namespace task {

Initial::Initial(
  entity::Task task_data, 
  common::db::Setting storage_setting, 
  common::db::Setting aggregate_setting,
  Scheduler* inner_scheduler)
  : Base(task_data, storage_setting, aggregate_setting),
    inner_scheduler_(inner_scheduler)    
{
}

Initial::~Initial()
{
  logger_.information(PrepareHeader("InitialAggregation") + 
    "] Has died.");
}

void Initial::run()
{
  
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information(PrepareHeader("InitialAggregation") + 
    "] Running. Aggregated period [" + current_ts + " - ...]");

  while(storage_.CountStationMeasurements(task_entity_.station_uid) <= 0) {
    logger_.warning("Sleeping for 60 seconds");
    boost::this_thread::sleep_for(boost::chrono::seconds(60));
  }

  // check if any measurements exists
  if(storage_.CountStationMeasurements(task_entity_.station_uid) > 0) {
    // find oldest measure for station_uid
    tm oldest_measure = storage_
      .GetOldestStationMeasureTime(task_entity_.station_uid);

    logger_.information(PrepareHeader("InitialAggregation") +
      "Oldest measure from " + asctime(&oldest_measure));

    time_t current_task_t = mktime(&task_entity_.current_ts);
    time_t oldest_measure_t = mktime(&oldest_measure);

    if (current_task_t < oldest_measure_t) {
      storage_.UpdateTaskCurrentTime(task_entity_.id, oldest_measure);
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

  logger_.information(PrepareHeader("InitialAggregation") + 
    "] Task ended.");  
}
// ~~ stormy::aggregation::task::Initial
}}}