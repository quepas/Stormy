#pragma once

#include <vector>
#include <Poco/Logger.h>
#include <Poco/Util/Timer.h> 

#include "../aggregation_entity_task.h"
#include "task/Base.h"
#include "task/Factory.h"

namespace stormy
{
	namespace aggregate
	{
		class Scheduler : public Poco::Util::Timer
		{
    public:
      explicit Scheduler(task::Factory& factory);			
			~Scheduler();

      void Schedule(std::vector<aggregation::entity::Task> task_entites);
      void ScheduleAsInitialTask(aggregation::entity::Task task);
      void ScheduleAsRegularTask(aggregation::entity::Task task);
      void Cancel();
      bool IsCancelled();

      static int GetTaskRefreshTime(std::string period_name);
    private:
      void Clear();      
    
      std::vector<task::Base*> scheduled_tasks_;
      task::Factory& factory_;
      Poco::Logger& logger_;
      bool cancelled_;
		};
	}
}