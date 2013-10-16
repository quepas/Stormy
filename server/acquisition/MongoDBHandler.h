#pragma once

#include <string>
#include <mongo/client/dbclient.h>

#include "../../common/MeteoData.h"

namespace Stormy
{
	class MongoDBHandler
	{
		public:
			MongoDBHandler(std::string dbAddress = "");
			~MongoDBHandler();

			void connect(std::string dbAddress);
			void insertMeteoData(MeteoData* meteoData);

			void setDbAndCollection(std::string dbName, std::string collectionName);			

		private:
			bool connected;		
			std::string currentDB, currentCollection;
			mongo::DBClientConnection connection;

			std::string createDbCollectionName();
	};
}