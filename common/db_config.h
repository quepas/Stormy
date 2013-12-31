#pragma once

#include <string>
#include "yaml_base_config.h"
#include "db_setting.h"

namespace stormy {
  namespace common {
    namespace db {

class Config : public yaml::BaseConfig
{
public:
	Config(std::string path);
  ~Config();
	Setting Configuration() {
		return configuration_;
	}
private:
	Setting configuration_;
	void MapIntoConfiguration();
};
// ~~ stormy::common::db::Config
}}}