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
	
	// metrics
	auto metrics = 
		AcquisitionHTTPConnector::getMetricsAt(host, port);
	dbStorage -> insertMetrics(metrics);

	// stations
	auto stations = 
		AcquisitionHTTPConnector::getStationsAt(host, port);
	dbStorage -> insertStations(stations);

	uint32 measurementCounter = 0;
	// data
	Utils::forEach(stations, [&](shared_ptr<Station> station) {
		auto newestMeasureForStation =
			dbStorage -> findNewestMeasureTimeByStationUID(station -> uid);

		auto measurements =
			AcquisitionHTTPConnector::getMeasurementsForStationNewerThanAt(
				host, port, station -> uid, newestMeasureForStation);
		measurementCounter += measurements.size();
		dbStorage -> insertMeasurements(measurements);		
	});
	cout << "[AcquisitionTask] Fetched " << measurementCounter 
		<< " measurements from " <<	server -> name << endl;
}
