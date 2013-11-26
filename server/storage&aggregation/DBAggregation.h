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
			std::vector<std::string> getPeriodNames();
			uint32 countPeriod();
			std::string getPeriodCorrectInterval(std::string name);
						
			uint32 createTask(uint32 station_id, AggregationSetting aggregation);			
			Poco::Timestamp taskCurrentTime(uint32 id);
			bool taskExsist(uint32 station_id, AggregationSetting aggregation);
			uint32 getTaskId(uint32 station_id, std::string period_name);
			//
			void increaseTaskTime(std::string periodName, uint32 id);
			
			DBStorage* getStorageDatabase() {
				return dbStorage;
			}
		private:
			uint32 insertTask(uint32 station_id, AggregationSetting aggregation);

			void connect();

			Database* dbAggregation;
			DBStorage* dbStorage;
			soci::session sql;
	};
}