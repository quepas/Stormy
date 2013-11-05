#pragma once

#include <string>
#include <mongo/client/dbclient.h>

#include "MeteoData.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "localhost");
			~MongoDBHandler();

			void connect(std::string dbAddress);

			void clearMeteosData();			
			void insertMeteoData(Meteo::Measurement* meteoData);
			std::vector<Meteo::Measurement*> getMeteoData(std::string stationId);
			Meteo::Measurement* getCurrentMeteoTypeData(std::string stationId, std::string typeId);
			std::vector<Meteo::Measurement*> getCurrentMeteoTypeDatas(std::string stationId, std::string typeId);

			void clearStationsData();
			void insertStationsData(std::vector<Meteo::Station*>& data);			
			void insertStationData(Meteo::Station* data);
			std::vector<Meteo::Station*> getStationsData();		

		private:
			bool connected;				
			mongo::DBClientConnection connection;		
	};
}