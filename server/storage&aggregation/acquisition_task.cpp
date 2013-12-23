#include "acquisition_task.h"

#include <iostream>
#include <cstdint>
#include <Poco/NumberFormatter.h>
#include <Poco/Stopwatch.h>

#include "../../common/entity_measurement.h"
#include "../../common/entity_station.h"
#include "acquisition_http_connector.h"
#include "acquisition_json_util.h"

using namespace stormy::common;
using std::string;
using std::vector;
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
	HTTPConnector http_connector(server->host, server->port);

	logger_.notice("[AcquisitionTask] Start fetching data from " + 
    server -> name);
	// metrics
	auto metrics = http_connector.FetchMetricsAt();
	dbStorage -> insertMetrics(metrics);

	// stations
	auto stations = http_connector.FetchStationsAt();
	dbStorage -> insertStations(stations);	

	uint32_t measurementCounter = 0;
	// data
	Stormy::Utils::forEach(stations, [&](entity::Station station) {
		auto newestMeasureForStation =
			dbStorage -> findNewestMeasureTimeByStationUID(station.uid);

		vector<entity::Measurement> measurements;		
		if(newestMeasureForStation.epochMicroseconds() != 0) {
			measurements = http_connector.FetchMeasurementsForStationNewerThanAt(
					station.uid, newestMeasureForStation);
		} else {
			measurements = http_connector.FetchMeasurementsForStationAt(
				station.uid);
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
