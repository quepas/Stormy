#include <iostream>
#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/AutoPtr.h>

#include "AggregationConfig.h"
#include "AggregationSetting.h"
#include "acquisition_config.h"
#include "DBStorage.h"
#include "DBAggregation.h"
#include "acquisition_scheduler.h"
#include "../../common/Utils.h"
#include "../../common/db_config.h"
#include "aggregate/Engine.h"
#include "rest_service.h"

using namespace stormy;
using namespace stormy::common;
using namespace Stormy;
using namespace Poco;

int main(int argc, char** argv) {
	AutoPtr<WindowsColorConsoleChannel> channel(new WindowsColorConsoleChannel);
  (*channel).setProperty("informationColor", "gray");   
	Logger::root().setChannel(channel);
	Logger& logger = Logger::get("aggregation_main_thread");  
	acquisition::Config acquisitionServersCfg("config/acquisition_servers.yaml");
	AggregationConfig aggregationCfg("config/aggregation.yaml");
	db::Config storageDBcfg("config/storage_database.yaml");
	db::Config aggregationDBcfg("config/aggregation_database.yaml");

	DBStorage storage(storageDBcfg.Configuration());
	DBAggregation aggregation(aggregationDBcfg.Configuration(), &storage);
  
  logger.information("==== Storage & Aggregation started. ====");	  
	logger.information("Measurements in storage: " + 
    NumberFormatter::format(storage.countAllMeasurements()));
	logger.information("Available stations: " + 
    NumberFormatter::format(storage.CountStations()));
	logger.information("-------------------------------------------------------------"
		"-------------------------------------------------------------");
	// display current configurations
	logger.information("=== Acquisition Servers: ");
	Utils::forEach(acquisitionServersCfg.Configuration(),
		[&](acquisition::Setting* server) {
			logger.information("\t" + server -> ToString());
	});
	logger.information("=== Available aggregates: ");
	Utils::forEach(aggregationCfg.getConfiguration(),
		[&](AggregationSetting setting) {
			logger.information("\t" + setting.toString());
	});
	logger.information("=== Storage database: ");
	logger.information("\t" + storageDBcfg.Configuration()->ToString());
	logger.information("=== Aggregation database: ");
	logger.information("\t" + aggregationDBcfg.Configuration()->ToString());
	logger.information("-------------------------------------------------------------"
		"-------------------------------------------------------------");

	/*AcquistionScheduler scheduler(&storage);
	scheduler.scheduleManyAcquisition(acquisitionServersCfg.getConfiguration());	

	stormy::aggregate::Engine aggregation_engine(&storage, &aggregation);
	aggregation_engine.Start(); */
  
  auto& rest_service = stormy::rest::Service(&storage, &aggregation);
  rest_service.run(argc, argv);
}