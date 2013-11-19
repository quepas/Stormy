#include "AcquisitionTask.h"

#include <iostream>
#include <Poco/NumberFormatter.h>

#include "../../common/data/Station.h"
#include "AcquisitionHTTPConnector.h"
#include "JSONUtils.h"

using namespace Stormy;
using namespace Stormy::Data;
using namespace Poco;
using namespace std;

AcquistionTask::AcquistionTask( DBStorage* _dbStorage, AcquisitionServer* _server)
	:	dbStorage(_dbStorage), server(_server)
{

}

AcquistionTask::~AcquistionTask()
{

}

void AcquistionTask::run()
{
	std::string host = server -> host;
	unsigned port = server -> port;

	cout << "[AcquisitionTask] Fetch data from server:\n\t" 
		<<	server -> toString() << endl;	
	
	auto stations = 
		AcquisitionHTTPConnector::getStationsAt(host, port);

	Utils::forEach(stations, [&](shared_ptr<Station> station) {
		cout << station -> uid << endl;
		auto measurements = 
			AcquisitionHTTPConnector::getMeasurementsForStationAt(host, port, station -> uid);
		dbStorage -> insertMeasurements(measurements);	
	});
}
