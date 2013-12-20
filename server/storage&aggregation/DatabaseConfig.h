#pragma once

#include <string>
#include "../../common/yaml_config.h"
#include "Database.h"

namespace Stormy
{
	class DatabaseConfig : public stormy::common::yaml::Config
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