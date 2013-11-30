#include <iostream>
#include <Poco/Logger.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/WindowsConsoleChannel.h>
#include <Poco/AutoPtr.h>

#include "AggregationConfig.h"
#include "AggregationSetting.h"
#include "AcquisitionServersConfig.h"
#include "DatabaseConfig.h"
#include "DBStorage.h"
#include "DBAggregation.h"
#include "AcquisitionScheduler.h"
#include "AggregationScheduler.h"
#include "../../common/Utils.h"
#include "../../common/data/Station.h"
#include "aggregate/Engine.h"

using namespace Stormy;
using namespace std;
using namespace Poco;

int main() {
	cout << "==== Storage & Aggregation started. ====" << endl;	
	AutoPtr<ConsoleChannel> channel(new ConsoleChannel);
	Logger::root().setChannel(channel);
	Logger& logger = Logger::get("aggregation");
	AcquisitionServersConfig acquisitionServersCfg("config/acquisition_servers.yaml");
	AggregationConfig aggregationCfg("config/aggregation.yaml");
	DatabaseConfig storageDBcfg("config/storage_database.yaml");
	DatabaseConfig aggregationDBcfg("config/aggregation_database.yaml");

	DBStorage storage(storageDBcfg.getConfiguration());
	DBAggregation aggregation(aggregationDBcfg.getConfiguration(), &storage);
	cout << "Measurements in storage: " << storage.countAllMeasurements() << endl;
	cout << "Available stations: " << storage.countStation() << endl;

	cout << "-------------------------------------------------------------"
		"-------------------------------------------------------------"<< endl;
	// display current configurations
	cout << "=== Acquisition Servers: " << endl;
	Utils::forEach(acquisitionServersCfg.getConfiguration(),
		[](AcquisitionServer* server) {
			cout << "\t" << server -> toString() << endl;
	});
	cout << "=== Available aggregates: " << endl;
	Utils::forEach(aggregationCfg.getConfiguration(),
		[](AggregationSetting setting) {
			cout << "\t" << setting.toString() << endl;
	});
	cout << "=== Storage database: " << endl;
	cout << "\t" << storageDBcfg.getConfiguration()->toString() << endl;
	cout << "=== Aggregation database: " << endl;
	cout << "\t" << aggregationDBcfg.getConfiguration()->toString() << endl;
	cout << "-------------------------------------------------------------"
		"-------------------------------------------------------------"<< endl;
	aggregation.insertPeriods(aggregationCfg.getConfiguration());
	AcquistionScheduler scheduler(&storage);
	scheduler.scheduleManyAcquisition(acquisitionServersCfg.getConfiguration());
	//AggregationScheduler aggregationScheduler(&aggregation);
	//aggregationScheduler.scheduleManyAggregations(aggregationCfg.getConfiguration());		
	stormy::aggregate::Engine aggregation_engine(storage);
	aggregation_engine.Start();

	getchar();
}