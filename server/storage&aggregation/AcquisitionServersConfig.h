#pragma once

#include <string>
#include "../../common/YAMLConfig.h"
#include "AcquisitionServer.h"

namespace Stormy 
{
	class AcquisitionServersConfig : public YAMLConfig
	{
		public:
			AcquisitionServersConfig(std::string path);
			~AcquisitionServersConfig();

			std::vector<AcquisitionServer*>& getConfiguration() {
				return configuration;
			}

		private:
			std::vector<AcquisitionServer*> configuration;
			void load();
	};
}