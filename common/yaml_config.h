#pragma once

#include <yaml-cpp/yaml.h>
#include <vector>
#include <map>
#include <cstdint>

namespace stormy {
  namespace common {
    namespace yaml {

class Config
{
	public:
		Config(std::string path);
		~Config();

		bool HasField(std::string field, uint16_t index = 0);
		std::vector<std::string> GetFields(uint16_t index = 0);
		uint16_t Size();

    std::string AsString(std::string field, uint16_t index = 0);
		int AsInt(std::string field, uint16_t index = 0);
		double AsDouble(std::string field, uint16_t index = 0);
		bool AsBool(std::string field, uint16_t index = 0);
			
	private:
		std::vector<std::map<std::string, std::string>> data_;
			
		void OpenFromFile(std::string path);
		void ReadAndAddEntry(YAML::Node entry);			
		bool CheckIndex(uint16_t index);
		std::string GetValue(std::string key, uint16_t index = 0);
};
// ~~ stormy::common::yaml::Config
}}}