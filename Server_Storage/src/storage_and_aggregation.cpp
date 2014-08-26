#include <iostream>
#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/AutoPtr.h>

#include "db_storage.h"
#include "acquisition_scheduler.h"
#include "aggregation_engine.h"
#include "rest_service.h"
#include "analysis_operation_mgr.h"

#include "util.h"
#include "db_config.h"

#include "settings.hpp"

using namespace stormy;
using namespace Poco;

int main(int argc, char** argv) {
  SetupLoggers();
  auto& logger = Logger::get("main");

  auto remote_servers = LoadRemoteServerSettings("config/remote_servers.json");
  auto aggregates = LoadAggregateSettings("config/aggregates.json");

  auto storage_db_setting = 
    common::db::Config("config/storage_database.yaml").Configuration();
	auto aggregate_db_setting = 
    common::db::Config("config/aggregation_database.yaml").Configuration();
  analysis::OperationMgr operation_mgr("./", aggregate_db_setting);

	db::Storage storage_for_acquisition(storage_db_setting);  	
  
  logger.information("==== Storage & Aggregation started. ====");	  
	logger.information("Measurements in storage: " + 
    NumberFormatter::format(storage_for_acquisition.CountAllMeasurements()));
	logger.information("Available stations: " + 
    NumberFormatter::format(storage_for_acquisition.CountAllStations()));
	logger.information(
    "-------------------------------------------------------------"
		"-------------------------------------------------------------");
  // display current configurations
  logger.information("=== Acquisition Servers: ");
  for (auto& server : remote_servers) {
    logger.information(ToString(server));
  }
	logger.information("=== Available aggregates: ");
  for (auto& entry : aggregates) {
    logger.information(ToString(entry));
  }
	logger.information("=== Storage database: ");
	logger.information("\t" + storage_db_setting.ToString());
	logger.information("=== Aggregation database: ");
	logger.information("\t" + aggregate_db_setting.ToString());
	logger.information(
    "-------------------------------------------------------------"
		"-------------------------------------------------------------");

	acquisition::Scheduler scheduler(&storage_for_acquisition);
	scheduler.Schedule(remote_servers);
  
	aggregation::Engine aggregation_engine(
    storage_db_setting, 
    aggregate_db_setting);
  aggregation::Engine::Restarter engine_restarter(300);
  engine_restarter(aggregation_engine);
  
  rest::Service rest_service(storage_db_setting, aggregate_db_setting);
  rest_service.run(argc, argv);
}