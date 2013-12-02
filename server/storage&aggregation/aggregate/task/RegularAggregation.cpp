#include "RegularAggregation.h"

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

RegularAggregation::RegularAggregation(entity::Task task_data)
  : Base(task_data)
{
}

RegularAggregation::~RegularAggregation()
{
  logger_.information("[aggregate::RegularAggregation#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Has died.");
}

void RegularAggregation::run()
{
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information("[aggregate::RegularAggregation#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Running. Aggregated period [" + current_ts + " - ...]");  

  logger_.information("[aggregate::RegularAggregation#" + 
    NumberFormatter::format(task_entity_.id) + "] Task ended.");  
}
// ~~ stormy::aggregate::task::RegularAggregation
}}}