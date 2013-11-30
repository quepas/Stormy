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

InitialAggregation::InitialAggregation( entity::Task task_data )
  : BaseTask(task_data)
{
}

InitialAggregation::~InitialAggregation()
{

}

void InitialAggregation::run()
{
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information("[aggregate::InitialAggregation#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Running. Aggregated period [" + current_ts + " - ...]");
}
// ~~ stormy::aggregate::task::InitialAggregation
}}}