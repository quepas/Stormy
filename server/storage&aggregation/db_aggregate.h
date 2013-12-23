#pragma once

#include <string>
#include <cstdint>
#include <soci.h>

#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "DBStorage.h"
#include "aggregation_setting.h"

#include "aggregation_entity_aggregate.h"

using namespace stormy::common;

namespace Stormy
{
	class DBAggregation
	{
		public:
			DBAggregation(db::Setting* database_setting, DBStorage* database_storage);
			~DBAggregation();
      
      // tasks
      uint32_t GetTaskId(std::string station_uid, std::string period_name);

      std::tm GetTaskCurrentTS(uint32_t task_id);
      std::string GetStationUIDFromTask(uint32_t task_id);     

      // aggregate
      bool InsertAggregate(stormy::aggregation::entity::Aggregate aggregate);

      // operation
      //bool InsertOperation(stormy::aggregate::entity::)
																
			DBStorage* database_storage() {
				return database_storage_;
			}
		private:  
			uint32_t InsertTask(std::string station_uid, stormy::aggregation::Setting aggregation_setting);

			void Connect();

			db::Setting* database_setting_;
			DBStorage* database_storage_;
			soci::session sql;
      Poco::Logger& logger_;
	};
}