#include "YAMLUtils.h"

using namespace Stormy;

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
