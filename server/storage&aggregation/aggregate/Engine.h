#pragma once

#include <string>
#include <Poco/Logger.h>

#include "../../../common/entity_station.h"
#include "../../../common/entity_metrics.h"

#include "Scheduler.h"
#include "task/Factory.h"
#include "../aggregation_entity_task.h"
#include "../aggregation_entity_period.h"
#include "../DBStorage.h"
#include "../DBAggregation.h"

namespace stormy
{
	namespace aggregate
	{
    enum BadTaskReason {
			USELESS,
			MISSING
		};

    class Engine
		{
		public:
      Engine(Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation);			
			~Engine();

			void Start();
			void Restart();
			void Stop();
						
		private:
      bool Init();			
      void Schedule();

			bool VerifyTasks();	
			void ClearVerificationData();
			bool IsPeriodStationVerified(std::string period_name, std::string station_uid);			
			bool FixTasks();
      void FetchAvailableData();
      void ClearAvailableData();								
      int FindAvailableTask(std::string period_name, std::string station_uid);
      bool CreateMissingTask(std::string period_name, std::string station_uid);
      bool DeleteUselessTask(std::string period_name, std::string station_uid);				

			// Available data
			std::vector<stormy::common::entity::Station> available_stations_;
			std::vector<stormy::common::entity::Metrics> available_metrics_;
			std::vector<aggregation::entity::Task> available_tasks_;
			std::vector<aggregation::entity::Period> available_periods_;

			// Verification data
			std::multimap<BadTaskReason, 
				std::pair<std::string, std::string>> bad_tasks_reason_;
			std::vector<std::pair<std::string, std::string>> verified_period_station_;

      task::Factory factory_;
      Scheduler scheduler_;
			Poco::Logger& logger_;
				
      Stormy::DBStorage* storage_;
      Stormy::DBAggregation* aggregation_;
		};
	}
}