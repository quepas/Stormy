#pragma once

#include <string>
#include <soci.h>
#include <Poco/Timestamp.h>

#include "../../common/data/Station.h"
#include "Database.h"
#include "DBStorage.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class DBAggregation
	{
		public:
			DBAggregation(Database* db, DBStorage* _dbStorage);
			~DBAggregation();

			bool insertPeriod(AggregationSetting setting);
			bool insertPeriods(std::vector<AggregationSetting> settings);
			
			// add param - startYear!?
			bool createTask(AggregationSetting aggregation, Data::Station station);
			
		private:
			void connect();

			Database* dbAggregation;
			DBStorage* dbStorage;
			soci::session sql;
	};
}