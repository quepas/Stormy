#pragma once

#include <string>
#include <mongo/client/dbclient.h>

#include "MeteoData.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "");
			~MongoDBHandler();

			void connect(std::string dbAddress);

			void clearMeteosData();			
			void insertMeteoData(Meteo::Measurement* meteoData);
			std::vector<Meteo::Measurement*> getMeteoData();

			void clearStationsData();
			void insertStationsData(std::vector<Meteo::Station*>& data);			
			void insertStationData(Meteo::Station* data);
			std::vector<Meteo::Station*> getStationsData();		

		private:
			bool connected;				
			mongo::DBClientConnection connection;		
	};
}