#pragma once

#include <Poco/Util/TimerTask.h>
#include <Poco/Timestamp.h>

#include "../../../common/Types.h"

namespace stormy
{
	namespace aggregate
	{
		class Task : public Poco::Util::TimerTask
		{
		public:
			Task();
			Task(Stormy::uint32 id);
			~Task();

			void run();

			// segment times
			Poco::Timestamp get_begin_ts();
			Poco::Timestamp get_end_ts();
		private:

			Poco::Timestamp currrentTS;

		};
	}
}