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
using std::vector;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::NumberParser;

namespace stormy {
  namespace aggregation {
    namespace task {

Regular::Regular(
  entity::Task task_data, 
  db::Storage& storage_database,
  db::Aggregate& aggregation_database)
  : Base(task_data, storage_database, aggregation_database)
{

}

Regular::~Regular()
{

}

void Regular::run()
{
  available_operations_ = aggregation_database_.GetOperations();
  // update task current_ts
  task_entity_.current_ts = aggregation_database_
    .GetTaskCurrentTS(task_entity_.id);
  tm station_last_update = storage_database_
    .GetStationLastUpdate(task_entity_.station_uid);
  time_t station_last_update_time = 
    mktime(&station_last_update);

  tm interval_end = aggregation_database_
    .CalculateAggregateEndTime(task_entity_.period_name, task_entity_.current_ts);
  time_t interval_start_time = mktime(&task_entity_.current_ts);
  time_t interval_end_time = mktime(&interval_end);

  if (station_last_update_time > interval_end_time) {
    const auto& measure_sets = storage_database_.GetMeasureSetsForStationBetweenTS(
      task_entity_.station_uid, 
      interval_start_time, 
      interval_end_time-1);
    const auto& metrics_sets = util::ConvertMeasureSetsToMetricsSets(measure_sets);
    for (auto ms_it = metrics_sets.begin(); ms_it != metrics_sets.end(); ++ms_it) {
      const auto& measure_data_set = ms_it->second;
      if (measure_data_set[0].is_numeric) {
        uint32_t sample_number = measure_data_set.size();

        /*
        PyObject* py_dataset = PyList_New(sample_number);
        // prepare sample dataset
        for (int av_i = 0; av_i < sample_number; ++av_i) {
          PyObject* py_value = 
            Py_BuildValue("d", measure_data_set[av_i].value_number);
          PyList_SetItem(py_dataset, av_i, py_value);
        }
        for (auto op_it = available_operations_.begin(); 
              op_it != available_operations_.end(); ++op_it) {
          string operation_name = op_it->name;
          PyObject* py_args = PyTuple_New(1);
          PyTuple_SetItem(py_args, 0, py_dataset);
          PyObject* py_result = nullptr;
            //common::py::Function(operation_name, operation_name)(py_args);

          if (py_result != nullptr) {
            double aggregated_value;
            aggregated_value= PyFloat_AsDouble(py_result);
            entity::Aggregate aggregate;
            aggregate.station_uid = task_entity_.station_uid;
            aggregate.metrics_code = ms_it->first;
            aggregate.operation_name = operation_name;
            aggregate.period_name = task_entity_.period_name;
            aggregate.start_time = task_entity_.current_ts;
            aggregate.value = aggregated_value;
            aggregate.sample_number = sample_number;
            aggregation_database_.InsertAggregate(aggregate);

            logger_.notice(PrepareHeader("Regular") + 
              task_entity_.period_name + " :" + string(ctime(&interval_start_time)) +
              + " (" + operation_name + " of " + ms_it->first + ": " + 
              NumberFormatter::format(aggregated_value) + "). Samp: " +
              NumberFormatter::format(sample_number));
            Py_DECREF(py_result);
          }
        }*/
      }
    }
    aggregation_database_.UpdateTaskCurrentTime(task_entity_.id, interval_end);
  }
}
// ~~ stormy::aggregation::task::Regular
}}}
