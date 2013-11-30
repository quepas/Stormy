#include "Engine.h"

// !DEBUG
#include <iostream>
// ~~

#include <Poco/NumberFormatter.h>

using Poco::Logger;
using Poco::NumberFormatter;
using std::string;
using std::make_pair;

namespace stormy {
	namespace aggregate {	

Engine::Engine( Stormy::DBStorage& storage )
	:	storage_(storage), logger_(Logger::get("aggregation"))
{	
}

Engine::~Engine()
{

}

void Engine::Start()
{
	Init();
}

void Engine::Pause()
{

}

void Engine::Stop()
{

}

// ~~ private ~~
void Engine::Init()
{	
	FetchAvailableData();

	// !DEBUG
	std::cout << "Stations: " << available_stations_.size() << std::endl;
	std::cout << "Metrics: " << available_metrics_.size() << std::endl;
	std::cout << "Tasks: " << available_tasks_.size() << std::endl;
	std::cout << "Periods: " << available_periods_.size() << std::endl;
	// !~~
	
	if (!VerifyTasks()) {
    logger_.warning("[aggregate::Engine] Some tasks are invalid. Fixing...");
		if(!FixTasks()) {
			logger_.error("[aggregate::Engine] Error while fixing invalid tasks.");
		}
		FetchAvailableData();
	}
}

bool Engine::VerifyTasks()
{
  ClearVerificationData();
  logger_.information("[aggregate::Engine] Tasks verification...");
  int count = 0;
	// check tasks for existence
	for (auto period_it = available_periods_.begin(); period_it != available_periods_.end(); ++period_it) {
    string period_name = period_it->name;		

		for (auto station_it = available_stations_.begin(); station_it != available_stations_.end(); ++station_it) {      
      string station_uid = station_it->uid;

			verified_period_station_.push_back(make_pair(period_name, station_uid));
			if(FindAvailableTask(period_name, station_uid) == -1) {
        ++count;
				bad_tasks_reason_.insert(
					make_pair(NONEEXISTENT, make_pair(period_name, station_uid)));				
			}
		}
	}  
  if(count > 0)
    logger_.warning("[aggregate::Engine] Number of NONEEXISTENT task: " + NumberFormatter::format(count));
	// check tasks for useless	
  count = 0;
	for (auto task_it = available_tasks_.begin(); task_it != available_tasks_.end(); ++task_it) {
		if(!IsPeriodStationVerified(task_it->period_name, task_it->station_uid)) {
      ++count;
      bad_tasks_reason_.insert(
        make_pair(USELESS, make_pair(task_it->period_name, task_it->station_uid)));
    }			
	}	
  if(count > 0)
    logger_.warning("[aggregate::Engine] Number of USELESS task: " + count);
	return bad_tasks_reason_.size() == 0;
}

void Engine::ClearVerificationData()
{
	bad_tasks_reason_.clear();
	verified_period_station_.clear();
}

bool Engine::IsPeriodStationVerified(std::string period_name, std::string station_uid)
{
	for (auto it = verified_period_station_.begin(); it != verified_period_station_.end(); ++it) {
		if(it -> first == period_name && it -> second == station_uid)
			return true;
	}
	return false;
}

bool Engine::FixTasks()
{
	for (auto it = bad_tasks_reason_.begin(); it != bad_tasks_reason_.end(); ++it) {
		string period_name = it->second.first, station_uid = it->second.second;

		switch (it->first) {
		case NONEEXISTENT:
			logger_.warning("[aggregate::Engine] Creating task for \'" + 
        period_name + "\' period and station \'" + station_uid);
      if(!CreateMissingTask(period_name, station_uid))
				return false;
			break;
		case USELESS:
			logger_.warning("[aggregate::Engine] Deleting useless task for \'" + 
        period_name + "\' period and station \'" + station_uid);
      if(!DeleteUselessTask(period_name, station_uid))
				return false;
			break;
		}
	}
	return true;
}

void Engine::FetchAvailableData()
{
	ClearAvailableData();
	available_stations_ = storage_.GetStations();
	available_metrics_ = storage_.GetMetrics();
	available_tasks_ = storage_.GetTasks();
	available_periods_ = storage_.GetPeriods();
}

void Engine::ClearAvailableData()
{
	available_stations_.clear();
	available_metrics_.clear();
	available_tasks_.clear();
	available_periods_.clear();
}

int Engine::FindAvailableTask( std::string period_name, std::string station_uid )
{
	for (int i = 0; i < available_tasks_.size(); ++i) {		
		if(available_tasks_[i].period_name == period_name &&
				available_tasks_[i].station_uid == station_uid)
			return i;
	}
	return -1;
}

bool Engine::CreateMissingTask( std::string period_name, std::string station_uid )
{
	return storage_.CreateTask(period_name, station_uid);
}

bool Engine::DeleteUselessTask( std::string period_name, std::string station_uid )
{
	return storage_.DeleteTask(period_name, station_uid);
}

// ~~ stormy::aggregation::Engine
}}