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
			Poco::Timestamp getPeriodAsSecond(std::string period_name);
			uint32 countPeriod();
			std::string getPeriodCorrectInterval(std::string name);
						
			uint32 createOrRefreshTask(uint32 station_id, AggregationSetting aggregation);			
			Poco::Timestamp taskCurrentTime(uint32 id);
			bool taskExsist(uint32 station_id, AggregationSetting aggregation);
			uint32 getTaskId(uint32 station_id, std::string period_name);
			uint32 getStationIdFromTask(uint32 id);
			bool setTaskTime(uint32 id, Poco::Timestamp time);			
			bool increaseTaskTimeBySeconds(uint32 id, ulong time);			
			
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