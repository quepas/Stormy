#pragma once

#include <string>

#include "acquisition_setting.h"
#include "yaml_base_config.h"

namespace stormy {
  namespace acquisition {

class Config : public common::yaml::BaseConfig
{
public:
	Config(std::string path);
	~Config();

  std::vector<Setting*>& Configuration() {
		return configuration_;
	}
		
private:
	std::vector<Setting*> configuration_;
	void MapIntoConfiguration();			
};
// ~~ stormy::acquisition::Config
}}
