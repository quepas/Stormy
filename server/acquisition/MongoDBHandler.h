#pragma once

#include <string>
#include <vector>
#include <mongo/client/dbclient.h>

#include "../../common/db_handler.h"
#include "../../common/db_has_data_expiration.h"
#include "../../common/entity_station.h"
#include "../../common/entity_metrics.h"
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
			void insertMeteoData(MeasurementPtr meteoData);
			MeasurementPtrVector getMeteoData(std::string stationId);
			MeasurementPtrVector getMeteoDataNewerThan(std::string stationId, std::string timestamp);
			MeasurementPtr getCurrentMeteoTypeData(std::string stationId, std::string typeId);
			MeasurementPtrVector getCurrentMeteoTypeDatas(std::string stationId, std::string typeId);

			void clearStationsData();
			void insertStationsData(const std::vector<entity::Station>& stations);
			void insertStationData(entity::Station station);
			StationPtrVector getStationsData();

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