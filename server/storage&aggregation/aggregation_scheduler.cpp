#include "aggregation_scheduler.h"

#include <map>
#include <Poco/NumberFormatter.h>
#include "aggregation_task_initial.h"
#include "aggregation_task_regular.h"

using std::map;
using std::string;
using std::vector;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregation {

Scheduler::Scheduler(task::Factory& factory)
  : logger_(Logger::get("aggregation")),
    factory_(factory),
    scheduled_tasks_(),    
    cancelled_(false)
{

}

Scheduler::~Scheduler()
{
  Cancel();
}

void Scheduler::Schedule(vector<entity::Task> task_entites)
{    
  factory_.SetInnerScheduler(this);
  logger_.information("[aggregate/Scheduler] Launching " + 
    NumberFormatter::format(task_entites.size()) + 
      " initial aggregation tasks.");
  cancelled_ = false;    
  for (auto it = task_entites.begin(); it != task_entites.end(); ++it) {     
    ScheduleAsInitialTask(*it);
  }  
}

void Scheduler::Cancel()
{  
  cancel();
  cancelled_ = true;
  logger_.information("[aggregate/Scheduler] Number of tasks cancelled: "
    + NumberFormatter::format(scheduled_tasks_.size()));  
  Clear();
}

// ~~ private ~~
int Scheduler::GetTaskRefreshTime(string period_name)
{
  static map<string, int> tasks_refresh_time_;  // In seconds
  tasks_refresh_time_["hourly"] = 3;
  tasks_refresh_time_["daily"] = 3600;
  tasks_refresh_time_["monthly"] = 36000;
  
  if(tasks_refresh_time_.find(period_name) == tasks_refresh_time_.end())
    return -1;
  return tasks_refresh_time_[period_name];
}

void Scheduler::Clear()
{  
  scheduled_tasks_.clear();
}

void Scheduler::ScheduleAsInitialTask(entity::Task task_entity)
{  
  auto scheduled_task = factory_
    .CreateDynamicTask(task::INITIAL, task_entity);
  scheduled_tasks_.push_back(scheduled_task);
  schedule(scheduled_task, 0);
}

void Scheduler::ScheduleAsRegularTask(entity::Task task_entity)
{  
  auto scheduled_task = factory_
    .CreateDynamicTask(task::REGULAR, task_entity);
  scheduled_tasks_.push_back(scheduled_task);
  schedule(scheduled_task, 0, 
            GetTaskRefreshTime(task_entity.period_name)*1000);
}
// ~~ stormy::aggregation::Scheduler
}}
