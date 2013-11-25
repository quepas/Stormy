#pragma once

#include <string>
#include <soci.h>
#include <Poco/Timestamp.h>

#include "Database.h"
#include "../../common/data/Station.h"
#include "../../common/data/Measurement.h"
#include "../../common/Utils.h"
#include "../../common/Types.h"

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(Database* storageDB);
			~DBStorage();

			// stations
			unsigned int countAllStation();
			void insertStation(StationPtr station);
			void insertStations(const StationPtrVector& stations);
			void clearAllStation();
			bool existsStationByUID(std::string uid);
			Data::Station* getStationByUID(std::string uid);
			uint32 getStationIdByUID(std::string uid);
			std::vector<uint32> getStationIds();
			uint32 countStation();

			// measurements
			bool insertMeasurements(
				const MeasurementPtrVector& measurements);
			Poco::Timestamp findNewestMeasureTimeByStationUID(std::string uid);
			Poco::Timestamp findOldestMeasureTimeByStationUID(std::string uid);
			bool existsAnyMeasurementFromStation(std::string uid);
			uint32 countAllMeasurements();

			// metrics
			bool insertOneMetrics(const MetricsPtr& metrics);
			bool insertMetrics(const MetricsPtrVector& metrics);
			bool existsMetricsByCode(const std::string& code);
			
		private:
			void connect();

			Database* configuration;
			soci::session sql;
	};
}