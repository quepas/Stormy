#include "acquisition_task.h"

#include <ctime>
#include <cstdint>
#include <map>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <Poco/NumberFormatter.h>
#include <Poco/Stopwatch.h>

#include "../../common/entity_measurement.h"
#include "../../common/entity_station.h"
#include "acquisition_http_connector.h"
#include "acquisition_json_util.h"

using namespace stormy::common;
using boost::copy;
using boost::adaptors::map_values;
using std::back_inserter;
using std::string;
using std::vector;
using std::map;
using std::time_t;
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
	dbStorage -> insertMetrics(metrics);

	// stations
	auto stations = http_connector.FetchStationsAt();
	dbStorage -> insertStations(stations);	

	uint32_t measurementCounter = 0;
	// data
	Each(stations, [&](entity::Station station) {
		auto newestMeasureForStation =
			dbStorage -> findNewestMeasureTimeByStationUID(station.uid);

		map<time_t, vector<entity::Measurement>> measurement_sets;		
		if(newestMeasureForStation.epochMicroseconds() != 0) {
			measurement_sets = http_connector.FetchMeasurementsForStationNewerThanAt(
					station.uid, newestMeasureForStation);
		} else {
			measurement_sets = http_connector.FetchMeasurementsForStationAt(
				station.uid);
		}
    vector<entity::Measurement> measures;
    for (auto it = measurement_sets.begin(); it != measurement_sets.end(); ++it) {
      auto measure_set = it->second;
      copy(measure_set, back_inserter(measures));
    }
		measurementCounter += measures.size();
		dbStorage -> InsertMeasurements(measures);		
		measurement_sets.clear();
	});
	logger_.notice("[acquisition/Task] Fetched " + 
    NumberFormatter::format(measurementCounter) + 
    " measurements. It took: " + 
    NumberFormatter::format(stopwatch.elapsed() / 1000.0) + "ms.");

	metrics.clear();
	stations.clear();
	stopwatch.stop();
}
// ~~ stormy::acquisition::Task
}}
