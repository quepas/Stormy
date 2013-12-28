#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <mongo/client/dbclient.h>

#include "../../common/db_handler.h"
#include "../../common/db_has_data_expiration.h"
#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
#include "../../common/entity_measurement.h"
#include "MeteoData.h"

using namespace stormy::common;

namespace Stormy
{
	class MongoDBHandler : public db::Handler, public db::HasDataExpiration
	{
		public:						
      void ExpireData() override;
      std::vector<std::string> FetchStationsUID();

			void clearMeteosData();
			void insertMeteoData(std::vector<entity::Measurement> meteoData);
			std::vector<entity::Measurement> getMeteoData(
        std::string stationId);

			void clearStationsData();
			void insertStationsData(const std::vector<entity::Station>& stations);
			void insertStationData(entity::Station station);
			std::vector<entity::Station> getStationsData();

      entity::Station GetStationByUID(std::string uid);
      uint32_t CountMeasureSetsForStationByUID(std::string uid);

      std::map<std::time_t, std::vector<entity::Measurement>> 
        GetMeasureSetsForStationBetweenTS(
          std::string station_uid, 
          std::time_t from, 
          std::time_t to);
      std::map<std::time_t, std::vector<entity::Measurement>> 
        GetMeasureSetsForStationAndTS(
        std::string station_uid, 
        std::time_t ts);
      std::map<std::time_t, std::vector<entity::Measurement>>
        GetAllMeasureSetsForStation(std::string station_uid);


			bool clearTypesData();
			bool insertTypesData(const std::vector<entity::Metrics>& metrics_vec);
			std::vector<entity::Metrics> getTypesData();			

			static MongoDBHandler& get() {
				static MongoDBHandler instance;
				return instance;
			}
		private:
			MongoDBHandler(std::string dbAddress = "localhost");
			MongoDBHandler(const MongoDBHandler&) : Handler("MongoDB") {}
			MongoDBHandler& operator=(const MongoDBHandler&) {}
			~MongoDBHandler();

			void connect(std::string dbAddress);			
			mongo::DBClientConnection connection;
	};
}