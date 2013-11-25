#pragma once

#include <Poco/Util/TimerTask.h>

#include "DBStorage.h"

namespace Stormy
{
	class AggregationTask : public Poco::Util::TimerTask
	{
	public:
		AggregationTask(DBStorage* _storage);
		~AggregationTask();

		void run();
	private:
		DBStorage* storage;
	};
}