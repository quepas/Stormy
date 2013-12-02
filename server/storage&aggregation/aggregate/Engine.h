#pragma once

#include <string>
#include <Poco/Logger.h>

#include "../../../common/data/Station.h"
#include "../../../common/data/Metrics.h"
#include "Scheduler.h"
#include "task/Factory.h"
#include "entity/Task.h"
#include "entity/Period.h"
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
			std::vector<Stormy::Data::Station> available_stations_;
			std::vector<Stormy::Data::Metrics> available_metrics_;
			std::vector<entity::Task> available_tasks_;
			std::vector<entity::Period> available_periods_;

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