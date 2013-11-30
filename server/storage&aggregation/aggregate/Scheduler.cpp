#include "Scheduler.h"

#include <map>
#include <Poco/NumberFormatter.h>
// !DEBUG
#include <iostream>
// ~~

using std::map;
using std::string;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregate {

Scheduler::Scheduler()
  : logger_(Logger::get("aggregation")),
    scheduled_tasks_(),    
    cancelled_(false)
{

}

Scheduler::~Scheduler()
{
  Clear();
}

void Scheduler::Schedule( std::vector<entity::Task> task_entites )
{  
  logger_.information("[aggregate::Schedule] Launching " + 
    NumberFormatter::format(task_entites.size()) + " tasks.");
  cancelled_ = false;    
  for (auto it = task_entites.begin(); it != task_entites.end(); ++it) {     
    Task* new_task = new Task(*it);
    scheduled_tasks_.push_back(new_task);
    schedule(new_task, 0, GetTaskRefreshTime(it->period_name) * 1000);
  }
}

void Scheduler::Cancel()
{  
  logger_.information("[aggregate::Schedule] Tasks cancelled.");
  cancelled_ = true;
  for (auto it = scheduled_tasks_.begin(); it != scheduled_tasks_.end(); ++it) {
    (*it)->cancel();
  }
  Clear();
}

// ~~ private ~~
int Scheduler::GetTaskRefreshTime( string period_name )
{
  static map<string, int> tasks_refresh_time_;  // In seconds
  tasks_refresh_time_["hourly"] = 10;
  tasks_refresh_time_["daily"] = 3600;
  tasks_refresh_time_["monthly"] = 36000;
  
  if(tasks_refresh_time_.find(period_name) == tasks_refresh_time_.end())
    return -1;
  return tasks_refresh_time_[period_name];
}

void Scheduler::Clear()
{  
  for (auto it = scheduled_tasks_.begin(); it != scheduled_tasks_.end(); ++it) {
    delete *it;
  }
  scheduled_tasks_.clear();
}

// ~~ stormy::aggregation::Scheduler
}}
