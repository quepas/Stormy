#pragma once

#include <string>
#include <cstdint>
#include <soci.h>

#include "Database.h"
#include "DBStorage.h"
#include "AggregationSetting.h"

#include "aggregate/entity/Aggregate.h"

namespace Stormy
{
	class DBAggregation
	{
		public:
			DBAggregation(Database* database_setting, DBStorage* database_storage);
			~DBAggregation();
      
      // tasks
      uint32_t GetTaskId(std::string station_uid, std::string period_name);

      std::tm GetTaskCurrentTS(uint32_t task_id);
      std::string GetStationUIDFromTask(uint32_t task_id);     

      // aggregate
      bool InsertAggregate(stormy::aggregate::entity::Aggregate aggregate);

      // operation
      //bool InsertOperation(stormy::aggregate::entity::)
																
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