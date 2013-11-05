#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

namespace Stormy
{
	class YAMLUtils
	{
		public:
			static bool isDefined(YAML::iterator it, std::string field);
			static std::string getString(YAML::iterator it, std::string field);
			static double getNumber(YAML::iterator it, std::string field);
			static long getLongNumber(YAML::iterator it, std::string field);
			static bool getBoolFromString(YAML::iterator it, std::string field);

		private:
	};
}