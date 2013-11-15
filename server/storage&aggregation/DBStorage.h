#pragma once

#include <string>
#include "StorageDatabase.h"

namespace Stormy
{
	class DBStorage
	{
		public:
			DBStorage(StorageDatabase* storageDB);
			~DBStorage();

			void connect();
		private:
			StorageDatabase* configuration;
	};
}