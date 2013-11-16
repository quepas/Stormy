#pragma once

#include <string>
#include <soci.h>

#include "StorageDatabase.h"
#include "../../common/data/Station.h"

#define TRY try {
#define CATCH }catch(const exception& ex){cout << ex.what() << endl;}
#define CATCH_MSG(msg) }catch(const exception& ex){cout << msg << ex.what() << endl;}

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(StorageDatabase* storageDB);
			~DBStorage();

			unsigned int countStation();
			void insertStation(Data::Station* station);

			void connect();
		private:
			StorageDatabase* configuration;
			soci::session sql;
	};
}