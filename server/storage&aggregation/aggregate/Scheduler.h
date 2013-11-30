#pragma once

#include <vector>
#include <Poco/Logger.h>
#include <Poco/Util/Timer.h> 

#include "Task.h"
#include "entity/Task.h"

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
      void Cancel();
      bool IsCancelled();

      static int GetTaskRefreshTime(std::string period_name);
    private:
      void Clear();
    
      std::vector<Task*> scheduled_tasks_;
      Poco::Logger& logger_;
      bool cancelled_;
		};
	}
}