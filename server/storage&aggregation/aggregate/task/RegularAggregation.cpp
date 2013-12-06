#include "RegularAggregation.h"

#include <ctime>
#include <string>
#include <iostream>
#include <Poco/NumberFormatter.h>
#include <Poco/NumberParser.h>

#include "../entity/Aggregate.h"

using std::string;
using std::asctime;
using std::cout;
using std::endl;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::NumberParser;

namespace stormy {
  namespace aggregate {
    namespace task {

RegularAggregation::RegularAggregation( entity::Task task_data, 
  Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation )
  : Base(task_data, storage, aggregation)
{

}

RegularAggregation::~RegularAggregation()
{
  logger_.information(prepareHeader("RegularAggregation") + 
    "] Has died.");
}

void RegularAggregation::run()
{
  // update task current_ts
  task_entity_.current_ts = aggregation_->GetTaskCurrentTS(task_entity_.id);
  tm station_last_update_ts = storage_->GetStationLastUpdate(task_entity_.station_uid);
  time_t station_last_update_time = mktime(&station_last_update_ts);

  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end
  time_t current_time = mktime(&task_entity_.current_ts);
  time_t intrval_end_time = 3600*2 + current_time;
  tm interval_end_ts = *gmtime(&intrval_end_time);

  // IMPORTANT!: check if end time is newer than last data in station
  if (station_last_update_time > intrval_end_time) {
    // for all 'measurement code' get once, do on all operations
    auto metrics_code = storage_->GetMetricsCodes();
    // IMPORTANT! : check if measurement is numeric

    for (auto it = metrics_code.begin(); it != metrics_code.end(); ++it) {  
      auto values = storage_->GetStationMeasure(task_entity_.station_uid, *it, task_entity_.current_ts, interval_end_ts);
      int count = values.size();

      // IMPORTANT!: if any measure exists
      if (count > 0) {    
        logger_.information(prepareHeader("RegularAggregation") + 
          "] Running. Aggregated period [" + current_ts + " - "
          + asctime(&interval_end_ts) + ". Number of samples: " + NumberFormatter::format(count));
  
        // !DEBUG
        string str_values = "[";
        for (auto it = values.begin(); it != values.end(); ++it) {
          str_values.append(*it);
          str_values.append(", ");
        }
        if(values.size() > 0)
          str_values.erase(str_values.length()-2, 2);
        str_values.append("]");
        cout << str_values << endl;
        // ~!DEBUG

        // calculate mean(x)
        double sum = 0.0;
        count = 0;
        for (auto it = values.begin(); it != values.end(); ++it) {
          sum += NumberParser::parseFloat(*it); 
          ++count;
        }
        double mean = sum / count;
        cout << "\n\tMean: " << mean << "\n" << endl;        

        if (!values.empty()) {
          entity::Aggregate aggregate;
          aggregate.station_uid = task_entity_.station_uid;
          aggregate.metrics_code = *it;
          aggregate.operation_name = "mean";
          aggregate.period_name = task_entity_.period_name;
          aggregate.start_time = task_entity_.current_ts;
          aggregate.value = mean;
          aggregate.sample_number = values.size();

          aggregation_->InsertAggregate(aggregate);
          logger_.information(prepareHeader("RegularAggregation") +
            "Aggregate created.");
        }
      }      
    }
    storage_->UpdateTaskCurrentTime(task_entity_.id, interval_end_ts);
  }

  logger_.information(prepareHeader("RegularAggregation") + 
    "Task ended.");  
}
// ~~ stormy::aggregate::task::RegularAggregation
}}}