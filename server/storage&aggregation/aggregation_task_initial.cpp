#include "aggregation_task_initial.h"

#include <ctime>
#include <string>
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
  db::Storage& storage_database,
  db::Aggregate& aggregation_database,
  Scheduler* inner_scheduler)
  : Base(task_data, storage_database, aggregation_database),
    inner_scheduler_(inner_scheduler)    
{
}

Initial::~Initial()
{

}

void Initial::run()
{  
  string task_curent_time_str = asctime(&task_entity_.current_ts);
  // Erase '\n' from end
  task_curent_time_str.erase(task_curent_time_str.length()-1);  

  logger_.information(PrepareHeader("Initial") + 
    "Period of interest from " + task_curent_time_str + ".");
  
  // check if any measurements exists
  if(storage_database_.CountStationMeasurements(task_entity_.station_uid) > 0) {
    // find oldest measure for station_uid
    tm oldest_measure = storage_database_
      .GetOldestStationMeasureTime(task_entity_.station_uid);

    string oldest_measure_str = asctime(&oldest_measure);
    oldest_measure_str.erase(oldest_measure_str.length()-1);
    logger_.information(PrepareHeader("Initial") +
      "Oldest measure from " + oldest_measure_str);

    time_t current_task_t = mktime(&task_entity_.current_ts);
    time_t oldest_measure_t = mktime(&oldest_measure);

    if (current_task_t < oldest_measure_t) {      
      oldest_measure.tm_min = 0;
      oldest_measure.tm_sec = 0;

      if (task_entity_.period_name == "daily") {
        oldest_measure.tm_hour = 0;
      }
      if (task_entity_.period_name == "monthly") {
        oldest_measure.tm_hour = 0;
        oldest_measure.tm_mday = 1;
      }           
      aggregation_database_.UpdateTaskCurrentTime(task_entity_.id, oldest_measure);
      task_entity_.current_ts = oldest_measure;
    }
  }  
  
  if (inner_scheduler_) {
    inner_scheduler_->ScheduleAsRegularTask(task_entity_);
  }
  else {
    logger_.warning(PrepareHeader("Initial") + 
      "Inner scheduler not set.");
  }

  logger_.information(PrepareHeader("Initial") + 
    " Task ended.");  
}
// ~~ stormy::aggregation::task::Initial
}}}