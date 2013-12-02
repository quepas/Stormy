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

      // new style
      uint32 GetTaskId(std::string station_uid, std::string period_name);
      std::string GetStationUIDFromTask(uint32 id);

      //
			bool insertPeriod(AggregationSetting setting);
			bool insertPeriods(std::vector<AggregationSetting> settings);
			std::vector<std::string> getPeriodNames();
			Poco::Timestamp getPeriodAsSecond(std::string period_name);
			uint32 countPeriod();
			std::string getPeriodCorrectInterval(std::string name);
						
			uint32 createOrRefreshTask(std::string station_uid, AggregationSetting aggregation);			
			Poco::Timestamp taskCurrentTime(uint32 id);
			bool taskExsist(std::string station_uid, AggregationSetting aggregation);
			
			
			bool setTaskTime(uint32 id, Poco::Timestamp time);			
			bool increaseTaskTimeBySeconds(uint32 id, ulong time);			
			
			DBStorage* getStorageDatabase() {
				return dbStorage;
			}
		private:
      // new style
			uint32 InsertTask(std::string station_uid, AggregationSetting aggregation);

			void Connect();

			Database* dbAggregation;
			DBStorage* dbStorage;
			soci::session sql;
	};
}