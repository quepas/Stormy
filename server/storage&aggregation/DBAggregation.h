#pragma once

#include <string>
#include <cstdint>
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
			DBAggregation(Database* database_setting, DBStorage* database_storage);
			~DBAggregation();
     
      uint32_t GetTaskId(std::string station_uid, std::string period_name);
      std::string GetStationUIDFromTask(uint32_t task_id);     
																
			DBStorage* database_storage() {
				return database_storage_;
			}
		private:  
			uint32_t InsertTask(std::string station_uid, AggregationSetting aggregation_setting);

			void Connect();

			Database* database_setting_;
			DBStorage* database_storage_;
			soci::session sql;
	};
}