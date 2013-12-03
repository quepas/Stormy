#include "RegularAggregation.h"

#include <ctime>
#include <string>
#include <iostream>
#include <Poco/NumberFormatter.h>

using std::string;
using std::asctime;
using std::cout;
using std::endl;
using Poco::Logger;
using Poco::NumberFormatter;

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
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end
  time_t current_time = mktime(&task_entity_.current_ts);
  time_t intreval = 3600*2 + current_time;
  tm interval_end_time = *gmtime(&intreval);

  // for all 'measurement code' get once, do on all operations
  auto values = storage_->GetStationMeasure(task_entity_.station_uid, "airTemperature", task_entity_.current_ts, interval_end_time);
  int count = values.size();

  logger_.information(prepareHeader("RegularAggregation") + 
    "] Running. Aggregated period [" + current_ts + " - "
    + asctime(&interval_end_time) + ". Number of samples: " + NumberFormatter::format(count));
  
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

  logger_.information(prepareHeader("RegularAggregation") + 
    "] Task ended.");  
}
// ~~ stormy::aggregate::task::RegularAggregation
}}}