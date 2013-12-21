#pragma once

#include <string>
#include <cstdint>
#include <soci.h>
#include <Poco/Timestamp.h>
#include <Poco/Logger.h>

#include "../../common/db_setting.h"
#include "../../common/data/Station.h"
#include "../../common/data/Measurement.h"
#include "../../common/Utils.h"
#include "../../common/Types.h"

#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"

#include "aggregation_entity_task.h"
#include "aggregation_entity_period.h"

using namespace stormy::common;

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(db::Setting* storageDB);
			~DBStorage();

			// new style
			std::vector<stormy::common::entity::Station> GetStations();
      uint32_t CountStations();
			std::vector<stormy::aggregation::entity::Task> GetTasks();
			std::vector<stormy::aggregation::entity::Period> GetPeriods();   
      std::vector<stormy::common::entity::Metrics> GetMetrics();
      std::vector<std::string> GetMetricsCodes();

			bool DeleteTask(uint16_t id);
			bool DeleteTask(std::string period_name, std::string station_uid);
			bool CreateTask(std::string period_name, std::string station_uid);
      bool UpdateTaskCurrentTime(uint32_t id, std::tm timestamp);
      std::tm CalculateAggregateEndTime(std::string period_name, std::tm start_time);

      bool UpdateStationLastUpdate(std::string station_uid, std::tm timestamp);
      std::tm GetStationLastUpdate(std::string station_uid);

      // station other methods
      std::vector<std::string> GetStationUIDs();
      std::string GetStationName(std::string uid);

      std::tm GetOldestStationMeasureTime(std::string uid);
      int CountStationMeasures(std::string uid);

      // TODO: fix this! need entity::Measurement
      std::vector<std::string> GetStationMeasure(
        std::string station_uid, 
        std::string metrics_code, 
        std::tm begin_time, 
        std::tm end_time);

      std::map<std::time_t, std::string> GetMeasurement(
        std::string station_uid, 
        std::string metrics_code,
        std::tm begin, 
        std::tm end);

      std::map<std::time_t, std::string> GetMeasurementFromLast(
        std::string station_uid, 
        std::string metrics_code, 
        uint16_t from_last_hours);

      // TODO: refactor this
      bool InsertMeasurements(
        const MeasurementPtrVector& measurements);

			// stations					 
			void insertStation(StationPtr station);
			void insertStations(const StationPtrVector& stations);
			void clearAllStation();
			bool existsStationByUID(std::string uid);
			Data::Station* getStationByUID(std::string uid);								

			// measurements			
			Poco::Timestamp findNewestMeasureTimeByStationUID(std::string uid);			
			Poco::Timestamp findOldestMeasureTimeByStationUID(std::string uid);
			bool existsAnyMeasurementFromStation(std::string uid);
			ulong countMeasurementFromStation(std::string uid);
			uint32 countAllMeasurements();

			// metrics
			bool insertOneMetrics(const MetricsPtr& metrics);
			bool insertMetrics(const MetricsPtrVector& metrics);
			bool existsMetricsByCode(const std::string& code);
			
		private:
			void connect();

			db::Setting* configuration;
			soci::session sql;
      Poco::Logger& logger_;
	};
}