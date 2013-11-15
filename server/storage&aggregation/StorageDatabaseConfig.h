#pragma once

#include <string>
#include "../../common/YAMLConfig.h"
#include "StorageDatabase.h"

namespace Stormy
{
	class StorageDatabaseConfig : public YAMLConfig
	{
		public:
			StorageDatabaseConfig(std::string path);
			~StorageDatabaseConfig();

			StorageDatabase* getConfiguration() {
				return configuration;
			}

		private:
			StorageDatabase* configuration;
			void mapIntoConfiguration();
	};
}