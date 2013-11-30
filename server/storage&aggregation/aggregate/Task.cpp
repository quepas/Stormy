#include "Task.h"

#include <ctime>
#include <string>
#include <Poco/NumberFormatter.h>

using std::string;
using std::asctime;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregate {

Task::Task( entity::Task task )
  : logger_(Logger::get("aggregation")),
    task_entity_(task)
{

}

Task::~Task()
{

}

void Task::run()
{
  string current_ts = asctime(&task_entity_.current_ts);
  current_ts.erase(current_ts.length()-1);  // Erase '\n' from end

  logger_.information("[aggregate::Task#" + 
    NumberFormatter::format(task_entity_.id) + 
    "] Running. Aggregated period [" + current_ts + " - ...]");
}

// ~~ stormy::aggregation::Task
}}

