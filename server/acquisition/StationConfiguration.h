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

			StationPtrVector getConfiguration()
			{ return configuration; }

		private:
			bool load(std::string filePath);

			StationPtrVector configuration;
		};
	}

}