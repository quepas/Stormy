#pragma once

#include <string>
#include <Poco/Logger.h>
#include <Poco/Util/Timer.h>
#include <Poco/Util/TimerTask.h>

#include "../../common/db_setting.h"
#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"

#include "aggregation_scheduler.h"
#include "aggregation_task_factory.h"
#include "aggregation_entity_task.h"
#include "aggregation_entity_period.h"
#include "db_storage.h"
#include "db_aggregate.h"

namespace stormy {
  namespace aggregation {

enum BadTaskReason {
  USELESS,
  MISSING
};

class Engine
{
public:
  // Re-init (re-fetch station data and validate aggregate_tasks)
  class CyclicRestart;
  class Restarter;

  Engine(
    common::db::Setting storage_setting, 
    common::db::Setting aggregate_setting);
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
  std::vector<entity::Task> available_tasks_;
  std::vector<entity::Period> available_periods_;

  // Verification data
  std::multimap<BadTaskReason, std::pair<std::string, std::string>> bad_tasks_reason_;
  std::vector<std::pair<std::string, std::string>> verified_period_station_;

  db::Storage storage_;
  db::Aggregate aggregation_;

  task::Factory factory_;
  Scheduler scheduler_;
  Poco::Logger& logger_;		
public:
  class CyclicRestart : public Poco::Util::TimerTask
  {
  public:
    CyclicRestart(Engine& engine)
      : engine_(engine) {};

    void run() override {
      engine_.Restart();
    };
  private:
    Engine& engine_;
  };

  class Restarter : public Poco::Util::Timer
  {
  public:
    Restarter(std::time_t restart_seconds)
      : restart_seconds_(restart_seconds) {};

    void operator()(Engine& engine) {
      std::time_t restart_miliseconds = restart_seconds_ * 1000;
      schedule(
        new CyclicRestart(engine), 
        15 * 1000, 
        static_cast<long>(restart_miliseconds));
    };
  private:
    std::time_t restart_seconds_;    
  };
};
// ~~ stormy::aggregation::Engine
}}
