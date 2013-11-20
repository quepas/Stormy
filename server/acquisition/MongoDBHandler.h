#pragma once

#include <string>
#include <mongo/client/dbclient.h>

#include "MeteoData.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:						
			void clearMeteosData();
			void insertMeteoData(Meteo::Measurement* meteoData);
			std::vector<Meteo::Measurement*> getMeteoData(std::string stationId);
			std::vector<Meteo::Measurement*> getMeteoDataNewerThan(std::string stationId, std::string timestamp);
			Meteo::Measurement* getCurrentMeteoTypeData(std::string stationId, std::string typeId);
			std::vector<Meteo::Measurement*> getCurrentMeteoTypeDatas(std::string stationId, std::string typeId);

			void clearStationsData();
			void insertStationsData(std::vector<Meteo::Station*>& data);
			void insertStationData(Meteo::Station* data);
			std::vector<Meteo::Station*> getStationsData();

			bool clearTypesData();
			bool insertTypesData(const TypePtrVector& data);

			static MongoDBHandler& get() {
				static MongoDBHandler instance;
				return instance;
			}
		private:
			MongoDBHandler(std::string dbAddress = "localhost");
			MongoDBHandler(const MongoDBHandler&) {}
			MongoDBHandler& operator=(const MongoDBHandler&) {}
			~MongoDBHandler();

			void connect(std::string dbAddress);

			bool connected;
			mongo::DBClientConnection connection;
	};
}