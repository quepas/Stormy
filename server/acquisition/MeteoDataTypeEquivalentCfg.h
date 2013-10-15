#pragma once

#include "yaml-cpp/yaml.h"
#include <string>
#include <vector>

#include "../../common/MeteoData.h"

namespace Stormy
{
	class MeteoDataTypeEquivalentCfg
	{
		public:
			MeteoDataTypeEquivalentCfg(std::string filePath);
			~MeteoDataTypeEquivalentCfg();

			bool reload();
			std::vector<std::string> getEquivalents(TYPE type);
			MeteoDataType getEquivalentsData(TYPE type);
			bool isEquivalent(std::string text);
		private:
			bool load(std::string filePath);

			std::vector<MeteoDataType*> equivalents;
	};
}