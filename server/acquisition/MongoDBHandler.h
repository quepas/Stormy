#pragma once

#include <string>
#include <mongo/client/dbclient.h>

#include "../../common/MeteoData.h"
#include "../../common/MeteoStation.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "");
			~MongoDBHandler();

			void connect(std::string dbAddress);

			void clearMeteosData();
			void insertMeteosData(std::vector<MeteoData*> meteoData);
			void insertMeteoData(MeteoData* meteoData);
			std::vector<MeteoData*> getMeteoData();

			void clearStationsData();
			void insertStationsData(std::vector<MeteoStation*>& data);			
			void insertStationData(MeteoStation* data);
			std::vector<MeteoStation*> getStationsData();

			MeteoData* getCurrentMeteoData(std::string stationId);
			MeteoData* getArchivalMeteoData(std::string stationId, long long timestamp);

			void setDbAndCollection(std::string dbName, std::string collectionName);			

		private:
			bool connected;		
			std::string currentDB, currentCollection;
			mongo::DBClientConnection connection;

			std::string createDbCollectionName();
	};
}