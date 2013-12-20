#pragma once

#include <string>
#include "../../common/yaml_config.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class AggregationConfig : public stormy::common::yaml::Config
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