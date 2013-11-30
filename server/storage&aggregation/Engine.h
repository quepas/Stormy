#pragma once

#include <string>
#include <Poco/Logger.h>

#include "../../common/data/Station.h"
#include "../../common/data/Metrics.h"
#include "Task.h"
#include "aggregate/entity/Task.h"
#include "aggregate/entity/Period.h"
#include "DBStorage.h"

namespace stormy
{
	namespace aggregate
	{
		enum BadTaskReason {
			USELESS,
			NONEEXISTENT	
		};

		class Engine
		{
		public:
			explicit Engine(Stormy::DBStorage& storage);
			~Engine();
		
			void Start();
			void Pause();
			void Stop();
						
		private:
			/*
			 *	Verify if all tasks have existing station_id, period_name etc.
			 */
			bool VerifyTasks();	
			void ClearVerificationData();
			bool IsPeriodStationVerified(std::string period_name, std::string station_uid);
			/*
			 *	Fix bad tasks based on ids from bad_tasks_
			 */
			bool FixTasks();
			/*
			 *	Load data from database (storage & aggregate)
			 */
			void FetchAvailableData();
			void ClearAvailableData();

			int FindAvailableTask(std::string period_name, std::string station_uid);

			bool CreateMissingTask(std::string period_name, std::string station_uid);
			bool DeleteUselessTask(std::string period_name, std::string station_uid);
			
			void Init();
			void Clear();

			// available data
			std::vector<Stormy::Data::Station> available_stations_;
			std::vector<Stormy::Data::Metrics> available_metrics_;
			std::vector<entity::Task> available_tasks_;
			std::vector<entity::Period> available_periods_;

			// verification data
			std::map<BadTaskReason, 
				std::pair<std::string, std::string>> bad_tasks_reason_;
			std::vector<std::pair<std::string, std::string>> verified_period_station_;

			Poco::Logger& logger_;

			// temp handler
			Stormy::DBStorage& storage_;
		};
	}
}