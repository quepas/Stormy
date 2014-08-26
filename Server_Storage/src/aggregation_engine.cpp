#include "aggregation_engine.h"

#include <Poco/NumberFormatter.h>

using Poco::Logger;
using Poco::NumberFormatter;
using std::string;
using std::make_pair;

namespace stormy {
	namespace aggregation {

Engine::Engine(DatabaseSetting storage_setting, DatabaseSetting aggregate_setting)
  : logger_(Logger::get("aggregation")),
    storage_(storage_setting),
    aggregation_(aggregate_setting),
    factory_(storage_, aggregation_),
    scheduler_(factory_)
{

}

Engine::~Engine()
{

}

void Engine::Start()
{
  logger_.information("[aggregate::Engine] Start.");
	if(!Init()) {
    logger_.critical("[aggregate::Engine] Initialization failed.");
    return;
  }
  Schedule();  
}

void Engine::Restart()
{
  logger_.warning("[aggregate::Engine] Restart.");
  Stop();
  Start();
}

void Engine::Stop()
{
  logger_.warning("[aggregate::Engine] Stop.");
  scheduler_.Cancel();
}

// ~~ private ~~
bool Engine::Init()
{
	FetchAvailableData();

  logger_.debug("Stations: " + 
    NumberFormatter::format(available_stations_.size()));
	logger_.debug("Metrics: " + 
    NumberFormatter::format(available_metrics_.size()));
	logger_.debug("Tasks: " +
    NumberFormatter::format(available_tasks_.size()));
	logger_.debug("Periods: " +
    NumberFormatter::format(available_periods_.size()));

	if (!VerifyTasks()) {
    logger_.warning("[aggregate/Engine] Some tasks are invalid. Fixing...");
		if(!FixTasks()) {
			logger_.error("[aggregate/Engine] Error while fixing invalid tasks.");
      return false;
		}
		FetchAvailableData();    
	}
  return true;
}

void Engine::Schedule()
{
  scheduler_.Schedule(available_tasks_);
}

bool Engine::VerifyTasks()
{
  ClearVerificationData();
  logger_.information("[aggregate/Engine] Tasks verification...");
  int count = 0;
	// check tasks for existence
	for (auto period_it = available_periods_.begin(); 
        period_it != available_periods_.end(); ++period_it) {
    string period_name = period_it->name;

		for (auto station_it = available_stations_.begin(); 
          station_it != available_stations_.end(); ++station_it) {
      string station_uid = station_it->uid;

			verified_period_station_.push_back(
        make_pair(period_name, station_uid));
			if(FindAvailableTask(period_name, station_uid) == -1) {
        ++count;
				bad_tasks_reason_.insert(
					make_pair(MISSING, make_pair(period_name, station_uid)));
			}
		}
	}
  if(count > 0) {
    logger_.warning("[aggregate/Engine] Number of MISSING task: " + 
    NumberFormatter::format(count));
  }
	// check tasks for useless
  count = 0;
	for (auto task_it = available_tasks_.begin(); 
        task_it != available_tasks_.end(); ++task_it) {
		if(!IsPeriodStationVerified(
        task_it->period_name, task_it->station_uid)) {
      ++count;
      bad_tasks_reason_.insert(
        make_pair(USELESS, 
          make_pair(task_it->period_name, task_it->station_uid)));
    }
	}
  if(count > 0)
    logger_.warning("[aggregate/Engine] Number of USELESS task: " + count);
	return bad_tasks_reason_.size() == 0;
}

void Engine::ClearVerificationData()
{
	bad_tasks_reason_.clear();
	verified_period_station_.clear();
}

bool Engine::IsPeriodStationVerified(
  std::string period_name, 
  std::string station_uid)
{
	for (auto it = verified_period_station_.begin(); 
    it != verified_period_station_.end(); ++it) {
		if(it->first == period_name && 
        it->second == station_uid) 
      return true;
	}
	return false;
}

bool Engine::FixTasks()
{
	for (auto it = bad_tasks_reason_.begin(); 
        it != bad_tasks_reason_.end(); ++it) {
		string period_name = it->second.first;
    string station_uid = it->second.second;

		switch (it->first) {
		case MISSING:
			logger_.warning("[aggregate/Engine] Creating missing task for \'" +
        period_name + "\' period and station \'" + station_uid);
      if(!CreateMissingTask(period_name, station_uid))
				return false;
			break;
		case USELESS:
			logger_.warning("[aggregate/Engine] Deleting useless task for \'" +
        period_name + "\' period and station \'" + station_uid);
      if(!DeleteUselessTask(period_name, station_uid))
				return false;
			break;
		}
	}
  ClearVerificationData();
	return true;
}

void Engine::FetchAvailableData()
{
	ClearAvailableData();
	available_stations_ = storage_.GetStations();
	available_metrics_ = storage_.GetMetrics();
	available_tasks_ = aggregation_.GetTasks();
	available_periods_ = aggregation_.GetPeriods();
}

void Engine::ClearAvailableData()
{
	available_stations_.clear();
	available_metrics_.clear();
	available_tasks_.clear();
	available_periods_.clear();
}

int Engine::FindAvailableTask(
  std::string period_name, 
  std::string station_uid )
{
	for (int i = 0; i < available_tasks_.size(); ++i) {
		if(available_tasks_[i].period_name == period_name &&
				available_tasks_[i].station_uid == station_uid)
			return i;
	}
	return -1;
}

bool Engine::CreateMissingTask(
  std::string period_name, 
  std::string station_uid)
{
	return aggregation_.CreateTask(period_name, station_uid);
}

bool Engine::DeleteUselessTask(
  std::string period_name, 
  std::string station_uid)
{
	return aggregation_.DeleteTask(period_name, station_uid);
}



// ~~ stormy::aggregation::Engine
}}