#pragma once

#include <string>
#include "../../common/yaml_base_config.h"
#include "AcquisitionServer.h"

namespace Stormy 
{
	class AcquisitionServersConfig : public stormy::common::yaml::BaseConfig
	{
		public:
			AcquisitionServersConfig(std::string path);
			~AcquisitionServersConfig();

			std::vector<AcquisitionServer*>& getConfiguration() {
				return configuration;
			}
			
		private:
			std::vector<AcquisitionServer*> configuration;
			void mapIntoConfiguration();			
	};
}