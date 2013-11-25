#pragma once

#include <string>
#include <soci.h>
#include <Poco/Timestamp.h>

#include "../../common/data/Station.h"
#include "Database.h"
#include "DBStorage.h"
#include "AggregationSettings.h"

namespace Stormy
{
	class DBAggregation
	{
		public:
			DBAggregation(Database* db, DBStorage* _dbStorage);
			~DBAggregation();

			bool insertPeriod(AggregationSettings setting);
			bool insertPeriods(std::vector<AggregationSettings> settings);
			
			// add param - startYear!?
			bool createTask(AggregationSettings aggregation, Data::Station station);
			
		private:
			void connect();

			Database* dbAggregation;
			DBStorage* dbStorage;
			soci::session sql;
	};
}