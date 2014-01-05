#include "acquisition_task.h"

#include <ctime>
#include <cstdint>
#include <map>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/NumberFormatter.h>
#include <Poco/Stopwatch.h>

#include "../../common/entity_measurement.h"
#include "../../common/entity_station.h"
#include "acquisition_http_connector.h"
#include "acquisition_json_util.h"

using namespace stormy::common;
using boost::copy;
using std::back_inserter;
using std::string;
using std::vector;
using std::map;
using std::time_t;
using std::tm;
using Poco::Logger;
using Poco::NumberFormatter;
using Poco::Stopwatch;

namespace stormy {
  namespace acquisition {

Task::Task( db::Storage* _dbStorage, Setting* _server)
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

	logger_.notice("[acquisition/Task] Start fetching data from " + 
    server -> name);
	// metrics
	auto metrics = http_connector.FetchMetricsAt();
	dbStorage -> InsertMetrics(metrics);

	// stations
	auto stations = http_connector.FetchStationsAt();
	dbStorage -> InsertStations(stations);	

	uint32_t measurementCounter = 0;
	// data
	Each(stations, [&](entity::Station station) {
		tm newestMeasureForStation =
			dbStorage -> GetNewestStationMeasureTime(station.uid);

		map<time_t, vector<entity::Measurement>> measurement_sets;
    time_t newest_measure_time = mktime(&newestMeasureForStation);
		if(newest_measure_time > 0) {
      // make time locale (UTC/GMT+1) => +3600
      // look for greater&equal => +1
			measurement_sets = http_connector
        .FetchMeasureSets(station.uid, newest_measure_time+3600+1);
		} else {
			measurement_sets = http_connector
        .FetchMeasureSets(station.uid, 0);
		}    
		measurementCounter += measurement_sets.size();
    dbStorage -> InsertMeasureSets(measurement_sets);		
		measurement_sets.clear();
	});
	logger_.notice("[acquisition/Task] Fetched " + 
    NumberFormatter::format(measurementCounter) + 
    " measure sets. It took: " + 
    NumberFormatter::format(stopwatch.elapsed() / 1000.0) + "ms.");

	metrics.clear();
	stations.clear();
	stopwatch.stop();
}
// ~~ stormy::acquisition::Task
}}
