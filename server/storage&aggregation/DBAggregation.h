#pragma once

#include <string>
#include <soci.h>
#include <Poco/Timestamp.h>

#include "../../common/data/Station.h"
#include "Database.h"
#include "AggregationSettings.h"

namespace Stormy
{
	class DBAggregation
	{
		public:
			DBAggregation(Database* db);
			~DBAggregation();

			bool insertAggregationPeriod(AggregationSettings aggregation);
			bool prepareAggregationTimesForPeriod(std::string period);
			
			// add param - startYear!?
			bool createAggregationTask(AggregationSettings aggregation, Data::Station station);
			
		private:
			void connect();

			Database* configuration;
			soci::session sql;
	};
}