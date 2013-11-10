#include "YAMLUtils.h"

#include <boost/algorithm/string.hpp>

using namespace Stormy;
using boost::to_lower;
using boost::contains;

bool YAMLUtils::isDefined( YAML::iterator it, std::string field )
{
	return (*it)[field].IsDefined();
}

std::string YAMLUtils::getString( YAML::iterator it, std::string field )
{
	return (*it)[field].as<std::string>();
}

double YAMLUtils::getNumber( YAML::iterator it, std::string field )
{
	return (*it)[field].as<double>();
}

long Stormy::YAMLUtils::getLongNumber( YAML::iterator it, std::string field )
{
	return (*it)[field].as<long>();
}

bool Stormy::YAMLUtils::getBoolFromString( YAML::iterator it, std::string field )
{
	std::string result = getString(it, field);
	to_lower(result);
	return result == "true" || result == "1";	
}
