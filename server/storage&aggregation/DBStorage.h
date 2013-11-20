#pragma once

#include <string>
#include <soci.h>

#include "StorageDatabase.h"
#include "../../common/data/Station.h"
#include "../../common/data/Measurement.h"
#include "../../common/Utils.h"
#include "../../common/Types.h"

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(StorageDatabase* storageDB);
			~DBStorage();

			// stations
			unsigned int countAllStation();
			void insertStation(StationPtr station);
			void insertStations(const StationPtrVector& stations);
			void clearAllStation();
			bool existsStationByUID(std::string uid);
			Data::Station* getStationByUID(std::string uid);

			// measurements
			bool insertMeasurements(
				const MeasurementPtrVector& measurements);

			void connect();
		private:
			StorageDatabase* configuration;
			soci::session sql;
	};
}