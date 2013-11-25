#pragma once

#include <string>
#include "../../common/YAMLConfig.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class AggregationConfig : public YAMLConfig
	{
	public:
		AggregationConfig(std::string path);
		~AggregationConfig();

		std::vector<AggregationSetting>& getConfiguration() {
			return configuration;
		}
	private:
		std::vector<AggregationSetting> configuration;
		void mapIntoConfiguration();
	};
}