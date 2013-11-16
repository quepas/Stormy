#include <iostream>

#include "AcquisitionServersConfig.h"
#include "StorageDatabaseConfig.h"
#include "DBStorage.h"
#include "../../common/Utils.h"
#include "../../common/data/Station.h"

using namespace Stormy;
using namespace std;

int main() {
	cout << "==== Storage & Aggregation started. ====" << endl;
	AcquisitionServersConfig acquisitionServersCfg("config/acquisition_servers.yaml");
	StorageDatabaseConfig storageDBcfg("config/storage_database.yaml");

	// display current configurations
	Utils::forEach(acquisitionServersCfg.getConfiguration(), 
		[](AcquisitionServer* server) {
			cout << server -> toString() << endl;
	});
	cout << storageDBcfg.getConfiguration()->toString() << endl;
	
	DBStorage storage(storageDBcfg.getConfiguration());	

	Data::Station* station = new Data::Station();
	station->id = "testID";
	station->name = "testName";
	station -> url = "testURL";
	station -> refreshTime = 11;

	storage.insertStation(station);
	cout << storage.countStation() << endl;

	getchar();
}