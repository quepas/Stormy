#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>
#include <Poco/Timestamp.h>

#include "../../../common/Types.h"
#include "entity/Task.h"

namespace stormy
{
	namespace aggregate
	{
		class Task : public Poco::Util::TimerTask
		{
		public:
			explicit Task(entity::Task task);			
			~Task();

			void run();

			// segment times
			Poco::Timestamp get_begin_ts();
			Poco::Timestamp get_end_ts();
		private:
      entity::Task task_entity_;
			Poco::Timestamp currrentTS_;

      Poco::Logger& logger_;
		};
	}
}