#pragma once

#include "yaml-cpp/yaml.h"
#include <vector>

#include "MeteoData.h"

namespace Stormy
{
	class MeteoStationsCfg
	{
		public:
			MeteoStationsCfg(std::string filePath);
			~MeteoStationsCfg();
			
			std::vector<Meteo::Station*> getConfiguration() 
				{ return configuration; }

		private:
			bool load(std::string filePath);						

			std::vector<Meteo::Station*> configuration;
	};
}