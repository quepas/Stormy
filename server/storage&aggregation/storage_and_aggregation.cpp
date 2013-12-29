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
  (*channel).setProperty("informationColor", "gray");   
  (*channel).setProperty("errorColor", "red");
	Logger::root().setChannel(channel);
	Logger& logger = Logger::get("aggregation_main_thread");  
	acquisition::Config acquisitionServersCfg("config/acquisition_servers.yaml");
	aggregation::Config aggregationCfg("config/aggregation.yaml");
	common::db::Config storageDBcfg("config/storage_database.yaml");
	common::db::Config aggregationDBcfg("config/aggregation_database.yaml");

	db::Storage storage(storageDBcfg.Configuration());
	db::Aggregate aggregation(aggregationDBcfg.Configuration(), &storage);
  
  logger.information("==== Storage & Aggregation started. ====");	  
	logger.information("Measurements in storage: " + 
    NumberFormatter::format(storage.countAllMeasurements()));
	logger.information("Available stations: " + 
    NumberFormatter::format(storage.CountStations()));
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
	logger.information("\t" + storageDBcfg.Configuration()->ToString());
	logger.information("=== Aggregation database: ");
	logger.information("\t" + aggregationDBcfg.Configuration()->ToString());
	logger.information(
    "-------------------------------------------------------------"
		"-------------------------------------------------------------");

	acquisition::Scheduler scheduler(&storage);
	scheduler.Schedule(acquisitionServersCfg.Configuration());	

	/*aggregation::Engine aggregation_engine(&storage, &aggregation);
	aggregation_engine.Start();*/
  
  auto& rest_service = rest::Service(&storage, &aggregation);
  rest_service.run(argc, argv);
}