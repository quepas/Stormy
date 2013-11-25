#pragma once

#include <Poco/Util/Timer.h>

#include "DBStorage.h"
#include "AggregationSettings.h"

namespace Stormy 
{
	class AggregationScheduler	: public Poco::Util::Timer
	{
	public:
		AggregationScheduler(DBStorage* _dbStorage);
		~AggregationScheduler();

		void scheduleAggregation(AggregationSettings* settings);
	private:
		DBStorage* dbStorage;
	};
}