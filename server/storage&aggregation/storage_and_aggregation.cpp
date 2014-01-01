#include <iostream>
#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/AutoPtr.h>

#include "aggregation_config.h"
#include "aggregation_setting.h"
#include "acquisition_config.h"
#include "db_storage.h"
#include "db_aggregate.h"
#include "acquisition_scheduler.h"
#include "../../common/util.h"
#include "../../common/db_config.h"
#include "aggregation_engine.h"
#include "rest_service.h"

using namespace stormy;
using namespace Poco;

int main(int argc, char** argv) {
	AutoPtr<WindowsColorConsoleChannel> channel(new WindowsColorConsoleChannel);  
	Logger::root().setChannel(channel);
	Logger& logger = Logger::get("aggregation_main_thread");  
	acquisition::Config acquisitionServersCfg("config/acquisition_servers.yaml");
	aggregation::Config aggregationCfg("config/aggregation.yaml");
	
  auto storage_db_setting = 
    common::db::Config("config/storage_database.yaml").Configuration();
	auto aggregate_db_setting = 
    common::db::Config("config/aggregation_database.yaml").Configuration();

	db::Storage storage_for_acquisition(storage_db_setting);
  db::Storage storage_for_aggregation(storage_db_setting);
  db::Storage storage_for_rest(storage_db_setting);
	db::Aggregate aggregation(aggregate_db_setting);
  db::Aggregate aggregation_for_rest(aggregate_db_setting);
  
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
	common::Each(acquisitionServersCfg.Configuration(),
		[&](acquisition::Setting* server) {
			logger.information("\t" + server -> ToString());
	});
	logger.information("=== Available aggregates: ");
	common::Each(aggregationCfg.Configuration(),
		[&](aggregation::Setting setting) {
			logger.information("\t" + setting.ToString());
	});
	logger.information("=== Storage database: ");
	logger.information("\t" + storage_db_setting.ToString());
	logger.information("=== Aggregation database: ");
	logger.information("\t" + aggregate_db_setting.ToString());
	logger.information(
    "-------------------------------------------------------------"
		"-------------------------------------------------------------");

	acquisition::Scheduler scheduler(&storage_for_acquisition);
	scheduler.Schedule(acquisitionServersCfg.Configuration());	

	aggregation::Engine aggregation_engine(
    storage_db_setting, 
    aggregate_db_setting);
  aggregation::Engine::Restarter engine_restarter(360);
  engine_restarter(aggregation_engine);
  
  rest::Service rest_service(storage_db_setting, aggregate_db_setting);
  rest_service.run(argc, argv);
}