#pragma once

#include <string>
#include <yaml-cpp/yaml.h>
#include <algorithm>

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

			template<typename T>
			static void forEach(YAML::Node& node, T func);
		
	};

	template<typename T>
	void Stormy::YAMLUtils::forEach( YAML::Node& node, T func )
	{
		std::for_each(node.begin(), node.end(), func);
	}

}