#pragma once

#include <Poco/Util/TimerTask.h>

#include "DBAggregation.h"
#include "AggregationTask.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class AggregationTask : public Poco::Util::TimerTask
	{
	public:
		AggregationTask(DBAggregation* _storage, AggregationSetting _setting, uint32 _taskId);
		~AggregationTask();

		void run();
	private:
		DBAggregation* aggregation;
		AggregationSetting setting;
		uint32 taskId;
	};
}