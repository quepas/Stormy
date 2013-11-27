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
			void insertStation(StationPtr station);
			void insertStations(const StationPtrVector& stations);
			void clearAllStation();
			bool existsStationByUID(std::string uid);
			Data::Station* getStationByUID(std::string uid);
			std::string getStationName(uint32 id);
			uint32 getStationIdByUID(std::string uid);
			std::vector<uint32> getStationIds();
			uint32 countStation();

			// measurements
			bool insertMeasurements(
				const MeasurementPtrVector& measurements);
			Poco::Timestamp findNewestMeasureTimeByStationUID(std::string uid);
			Poco::Timestamp findNewestMeasureTimeFromStation(uint32 id);
			Poco::Timestamp findOldestMeasureTimeByStationUID(uint32 id);
			bool existsAnyMeasurementFromStation(std::string uid);
			ulong countMeasurementFromStation(uint32 id);
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