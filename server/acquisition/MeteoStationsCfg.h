#pragma once

#include "yaml-cpp/yaml.h"
#include <vector>

#include "../../common/MeteoStation.h"

namespace Stormy
{
	class MeteoStationsCfg
	{
		public:
			MeteoStationsCfg(std::string filePath);
			~MeteoStationsCfg();

			bool reload();
			std::vector<MeteoStation*> getConfiguration() 
				{ return configuration; }

		private:
			bool load(std::string filePath);
			bool loadSingleStation(YAML::iterator it);
			bool checkIfFieldDefined(YAML::iterator it, std::string field);

			std::vector<MeteoStation*> configuration;
	};
}