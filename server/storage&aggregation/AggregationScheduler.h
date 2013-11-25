#pragma once

#include <Poco/Util/Timer.h>

#include "DBAggregation.h"
#include "AggregationSetting.h"

namespace Stormy 
{
	class AggregationScheduler	: public Poco::Util::Timer
	{
	public:
		AggregationScheduler(DBAggregation* _dbAggregation);
		~AggregationScheduler();

		void scheduleAggregation(AggregationSetting setting);
		void scheduleManyAggregations(std::vector<AggregationSetting> const& settings);
	private:
		DBAggregation* dbAggregation;
	};
}