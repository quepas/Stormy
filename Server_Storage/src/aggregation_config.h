#pragma once

#include <string>

#include "yaml_base_config.h"
#include "aggregation_setting.h"

namespace stormy {
  namespace aggregation {

class Config : public common::yaml::BaseConfig
{
public:
	Config(std::string path);
	~Config();

	std::vector<Setting>& Configuration() {
		return configuration_;
	}
private:
	std::vector<Setting> configuration_;
	void MapIntoConfiguration();
};
// ~~ stormy::aggregation::Config
}}
