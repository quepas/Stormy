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
	Utils::forEach(stations, [&](StationPtr station) {
		auto newestMeasureForStation =
			dbStorage -> findNewestMeasureTimeByStationUID(station -> uid);

		MeasurementPtrVector measurements;		
		if(newestMeasureForStation.epochMicroseconds() != 0) {
			measurements = 
				AcquisitionHTTPConnector::getMeasurementsForStationNewerThanAt(
					host, port, station -> uid, newestMeasureForStation);
		} else {
			measurements = 
				AcquisitionHTTPConnector::getMeasurementsForStationAt(
				host, port, station -> uid);
		}
		measurementCounter += measurements.size();
		dbStorage -> insertMeasurements(measurements);		
		measurements.clear();
	});
	cout << "[AcquisitionTask] Fetched " << measurementCounter 
		<< " measurements. It took: " << stopwatch.elapsed() / 1000.0
		<< "ms." << endl;

	metrics.clear();
	stations.clear();
	stopwatch.stop();
}
