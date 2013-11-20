#include "AcquisitionTask.h"

#include <iostream>
#include <Poco/NumberFormatter.h>
#include <Poco/Stopwatch.h>

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
	Stopwatch stopwatch;
	stopwatch.start();
	std::string host = server -> host;
	unsigned port = server -> port;
	
	cout << "[AcquisitionTask] Start fetching data from " 
		<< server -> name << endl;
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
		<< " measurements. It took: " << stopwatch.elapsed() / 1000.0
		<< "ms." << endl;
	stopwatch.stop();
}
