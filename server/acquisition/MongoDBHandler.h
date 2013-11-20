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
			void insertMeteoData(MeasurementPtr meteoData);
			MeasurementPtrVector getMeteoData(std::string stationId);
			MeasurementPtrVector getMeteoDataNewerThan(std::string stationId, std::string timestamp);
			MeasurementPtr getCurrentMeteoTypeData(std::string stationId, std::string typeId);
			MeasurementPtrVector getCurrentMeteoTypeDatas(std::string stationId, std::string typeId);

			void clearStationsData();
			void insertStationsData(const StationPtrVector& data);
			void insertStationData(StationPtr data);
			StationPtrVector getStationsData();

			bool clearTypesData();
			bool insertTypesData(const TypePtrVector& data);
			TypePtrVector getTypesData();			

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