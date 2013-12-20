#include "yaml_util.h"

#include <boost/algorithm/string.hpp>

using std::string;
using YAML::iterator;
using boost::to_lower;

namespace stormy {
  namespace common {
    namespace yaml {

bool Util::IsDefined(iterator it, string field)
{
	return (*it)[field].IsDefined();
}

string Util::GetString(iterator it, string field)
{
	return (*it)[field].as<string>();
}

double Util::GetNumber(iterator it, string field)
{
	return (*it)[field].as<double>();
}

long Util::GetLongNumber(iterator it, string field)
{
	return (*it)[field].as<long>();
}

bool Util::GetBool(iterator it, string field)
{
	string result = GetString(it, field);
	to_lower(result);
	return result == "true" || result == "1";	
}
// ~~ stormy::common::yaml::Util
}}}
