#pragma once

#include <string>
#include "../../common/YAMLConfig.h"
#include "Database.h"

namespace Stormy
{
	class DatabaseConfig : public YAMLConfig
	{
		public:
			DatabaseConfig(std::string path);
			~DatabaseConfig();

			Database* getConfiguration() {
				return configuration;
			}

		private:
			Database* configuration;
			void mapIntoConfiguration();
	};
}