#pragma once

#include "yaml-cpp/yaml.h"
#include <string>
#include <vector>

#include "MeteoData.h"

namespace Stormy
{
	class EquivalentsConfig
	{
		public:
			EquivalentsConfig(std::string filePath);
			~EquivalentsConfig();

			bool reload();
			std::vector<std::string> getEquivalentsForType(TYPE type);
			MeteoDataType* getMeteoDataTypeForType(TYPE type);
			bool areEquivalentsForType(TYPE type, std::string text);
			TYPE getTypeByEquivalent(std::string text);
		private:
			bool load(std::string filePath);

			std::vector<MeteoDataType*> equivalents;
	};
}