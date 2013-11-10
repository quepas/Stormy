#pragma once

#include <yaml-cpp/yaml.h>
#include <vector>
#include <map>

namespace Stormy
{
	class YAMLConfig
	{
		public:
			YAMLConfig(std::string path);
			~YAMLConfig();
		
			bool hasField(std::string field, unsigned int index = 0);
			std::vector<std::string> getFields(unsigned int index = 0);
			unsigned int Size();

			std::string asString(std::string field, unsigned int index = 0);
			int asInt(std::string field, unsigned int index = 0);
			double asDouble(std::string field, unsigned int index = 0);
			bool asBool(std::string field, unsigned int index = 0);
			
		private:
			std::vector<std::map<std::string, std::string>> data;
			
			void openFromFile(std::string path);
			void readAndAddEntry(YAML::Node entry);			
			bool checkIndex(unsigned int index);
			std::string getValue(std::string key, unsigned int index = 0);
	};
}