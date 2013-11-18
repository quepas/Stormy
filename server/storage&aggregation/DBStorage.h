#pragma once

#include <string>
#include <soci.h>

#include "StorageDatabase.h"
#include "../../common/data/Station.h"
#include "../../common/Utils.h"

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(StorageDatabase* storageDB);
			~DBStorage();

			unsigned int countAllStation();
			void insertStation(Data::Station* station);
			void insertStations(const std::vector<Data::Station*>& stations);
			void clearAllStation();
			bool existsStationByUID(std::string uid);
			Data::Station* getStationByUID(std::string uid);

			void connect();
		private:
			StorageDatabase* configuration;
			soci::session sql;
	};
}