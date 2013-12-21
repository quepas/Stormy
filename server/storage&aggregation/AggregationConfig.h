#pragma once

#include <string>
#include "../../common/yaml_base_config.h"
#include "AggregationSetting.h"

namespace Stormy
{
	class AggregationConfig : public stormy::common::yaml::BaseConfig
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