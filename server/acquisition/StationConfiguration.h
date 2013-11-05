#pragma once

#include "yaml-cpp/yaml.h"
#include <vector>

#include "MeteoData.h"

namespace Stormy
{
	namespace Meteo
	{
		class StationConfiguration
		{
		public:
			StationConfiguration(std::string filePath);
			~StationConfiguration();

			std::vector<Station*> getConfiguration()
			{ return configuration; }

		private:
			bool load(std::string filePath);

			std::vector<Station*> configuration;
		};
	}

}