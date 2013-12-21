#include "acquisition_task.h"

#include <iostream>
#include <cstdint>
#include <Poco/NumberFormatter.h>
#include <Poco/Stopwatch.h>

#include "../../common/data/Station.h"
#include "acquisition_http_connector.h"
#include "JSONUtils.h"

using std::string;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::Stopwatch;

namespace stormy {
  namespace acquisition {

Task::Task( Stormy::DBStorage* _dbStorage, Setting* _server)
	:	logger_(Logger::get("acquisition/task")),
    dbStorage(_dbStorage), server(_server)
{

}

Task::~Task()
{

}

void Task::run()
{
	Stopwatch stopwatch;
	stopwatch.start();
	string host = server -> host;
	unsigned port = server -> port;
	
	logger_.notice("[AcquisitionTask] Start fetching data from " + 
    server -> name);
	// metrics
	auto metrics = HTTPConnector::getMetricsAt(host, port);
	dbStorage -> insertMetrics(metrics);

	// stations
	auto stations = HTTPConnector::getStationsAt(host, port);
	dbStorage -> insertStations(stations);	

	uint32_t measurementCounter = 0;
	// data
	Stormy::Utils::forEach(stations, [&](Stormy::StationPtr station) {
		auto newestMeasureForStation =
			dbStorage -> findNewestMeasureTimeByStationUID(station -> uid);

		Stormy::MeasurementPtrVector measurements;		
		if(newestMeasureForStation.epochMicroseconds() != 0) {
			measurements = HTTPConnector::getMeasurementsForStationNewerThanAt(
					host, port, station -> uid, newestMeasureForStation);
		} else {
			measurements = HTTPConnector::getMeasurementsForStationAt(
				host, port, station -> uid);
		}
		measurementCounter += measurements.size();
		dbStorage -> InsertMeasurements(measurements);		
		measurements.clear();
	});
	logger_.notice("[AcquisitionTask] Fetched " + 
    NumberFormatter::format(measurementCounter) + 
    " measurements. It took: " + 
    NumberFormatter::format(stopwatch.elapsed() / 1000.0) + "ms.");

	metrics.clear();
	stations.clear();
	stopwatch.stop();
}
// ~~ stormy::acquisition::Task
}}
