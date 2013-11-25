#pragma once

#include <Poco/Util/TimerTask.h>

#include "DBStorage.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class AggregationTask : public Poco::Util::TimerTask
	{
	public:
		AggregationTask(DBStorage* _storage, AggregationSetting _setting);
		~AggregationTask();

		void run();
	private:
		DBStorage* storage;
		AggregationSetting setting;
	};
}