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
	common::db::Config storageDBcfg("config/storage_database.yaml");
	common::db::Config aggregationDBcfg("config/aggregation_database.yaml");

	db::Storage storage_for_acquisition(storageDBcfg.Configuration());
  db::Storage storage_for_aggregation(storageDBcfg.Configuration());
  db::Storage storage_for_rest(storageDBcfg.Configuration());
	db::Aggregate aggregation(
    &aggregationDBcfg.Configuration(), &storage_for_aggregation);
  db::Aggregate aggregation_for_rest(
    &aggregationDBcfg.Configuration(), &storage_for_rest);
  
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
	logger.information("\t" + storageDBcfg.Configuration().ToString());
	logger.information("=== Aggregation database: ");
	logger.information("\t" + aggregationDBcfg.Configuration().ToString());
	logger.information(
    "-------------------------------------------------------------"
		"-------------------------------------------------------------");

	acquisition::Scheduler scheduler(&storage_for_acquisition);
	scheduler.Schedule(acquisitionServersCfg.Configuration());	

	aggregation::Engine aggregation_engine(&storage_for_aggregation, &aggregation);
  aggregation::Engine::Restarter asd(360);
  asd(aggregation_engine);	
  
  auto& rest_service = rest::Service(&storage_for_rest, &aggregation_for_rest);
  rest_service.run(argc, argv);
}