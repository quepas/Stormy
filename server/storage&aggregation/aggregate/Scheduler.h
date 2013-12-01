#pragma once

#include <vector>
#include <Poco/Logger.h>
#include <Poco/Util/Timer.h> 

#include "entity/Task.h"
#include "task/BaseTask.h"

namespace stormy
{
	namespace aggregate
	{
		class Scheduler : public Poco::Util::Timer
		{
    public:
			Scheduler();
			~Scheduler();

      void Schedule(std::vector<entity::Task> task_entites);
      void ScheduleAsInitialTask(entity::Task task);
      void ScheduleAsRegularTask(entity::Task task);
      void Cancel();
      bool IsCancelled();

      static int GetTaskRefreshTime(std::string period_name);
    private:
      void Clear();      
    
      std::vector<task::BaseTask*> scheduled_tasks_;
      Poco::Logger& logger_;
      bool cancelled_;
		};
	}
}