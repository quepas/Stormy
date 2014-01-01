#include "aggregation_task_regular.h"

#include <ctime>
#include <string>
#include <iostream>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>

#include "aggregation_entity_aggregate.h"
#include "aggregation_util.h"

using std::string;
using std::asctime;
using std::cout;
using std::endl;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::NumberParser;

namespace stormy {
  namespace aggregation {
    namespace task {

Regular::Regular(
  entity::Task task_data, 
  common::db::Setting storage_setting, 
  common::db::Setting aggregate_setting)
  : Base(task_data, storage_setting, aggregate_setting)
{

}

Regular::~Regular()
{

}

void Regular::run()
{
  // update task current_ts
  task_entity_.current_ts = aggregation_.GetTaskCurrentTS(
    task_entity_.id);
  tm station_last_update_ts = storage_.GetStationLastUpdate(
    task_entity_.station_uid);
  time_t station_last_update_time = mktime(&station_last_update_ts);

  string asc_current = asctime(&task_entity_.current_ts);
  asc_current.erase(asc_current.length()-1);  // Erase '\n' from end
  time_t current_time = mktime(&task_entity_.current_ts);
  time_t intrval_end_time = 3600*2 + current_time;
  tm interval_end_ts = *gmtime(&intrval_end_time);
  string asc_end = asctime(&interval_end_ts);

  // IMPORTANT!: check if end time is newer than last data in station
  if (station_last_update_time > intrval_end_time) {
    // for all 'measurement code' get once, do on all operations
    //const auto& metrics_code = storage_->GetMetricsCodes();
    // IMPORTANT! : check if measurement is numeric

    const auto& measure_sets = storage_.GetMeasureSetsForStationBetweenTS(
      task_entity_.station_uid, 
      current_time, 
      intrval_end_time-1);
    const auto& metrics_sets = util::ConvertMeasureSetsToMetricsSets(measure_sets);

    for (auto ms_it = metrics_sets.begin(); ms_it != metrics_sets.end(); ++ms_it) {
      const auto& measure_data_set = ms_it->second;

      if (measure_data_set[0].is_numeric) {
        // TODO: Loop for N operations
        double dataset_sum = 0.0;        
        double aggregate_value = 0.0;
        uint32_t sample_number = measure_data_set.size();
        for (auto av_it = measure_data_set.begin(); av_it != measure_data_set.end(); ++av_it) {
          dataset_sum += av_it->value_number;
        }
        aggregate_value = dataset_sum / measure_data_set.size();

        entity::Aggregate aggregate;
        aggregate.station_uid = task_entity_.station_uid;
        aggregate.metrics_code = ms_it->first;
        aggregate.operation_name = "mean";
        aggregate.period_name = task_entity_.period_name;
        aggregate.start_time = task_entity_.current_ts;
        aggregate.value = aggregate_value;
        aggregate.sample_number = sample_number;        
        aggregation_.InsertAggregate(aggregate);

        logger_.notice(PrepareHeader("Regular") + 
          "Period " + task_entity_.period_name + 
          " [" + asc_current + "-" + asc_end + "]. " + 
          "Samples: " + NumberFormatter::format(measure_data_set.size()) 
          + ". Mean: " + NumberFormatter::format(aggregate_value) +
          ". Station: " + task_entity_.station_uid);
      }      
    }
    storage_.UpdateTaskCurrentTime(task_entity_.id, interval_end_ts);
  }  
}
// ~~ stormy::aggregation::task::Regular
}}}