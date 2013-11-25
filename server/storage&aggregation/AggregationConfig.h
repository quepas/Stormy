#pragma once

#include <string>
#include "../../common/YAMLConfig.h"
#include "AggregationSettings.h"

namespace Stormy
{
	class AggregationConfig : public YAMLConfig
	{
	public:
		AggregationConfig(std::string path);
		~AggregationConfig();

		std::vector<AggregationSettings>& getConfiguration() {
			return configuration;
		}
	private:
		std::vector<AggregationSettings> configuration;
		void mapIntoConfiguration();
	};
}