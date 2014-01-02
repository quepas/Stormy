#include "yaml_util.h"

#include <boost/algorithm/string.hpp>

using std::string;
using YAML::iterator;
using boost::to_lower;

namespace stormy {
  namespace common {
    namespace yaml {

bool IsDefined(iterator it, string field)
{
  return (*it)[field].IsDefined();
}

string GetString(iterator it, string field)
{
  return (*it)[field].as<string>();
}

double GetNumber(iterator it, string field)
{
  return (*it)[field].as<double>();
}

long GetLongNumber(iterator it, string field)
{
  return (*it)[field].as<long>();
}

bool GetBool(iterator it, string field)
{
  string result = GetString(it, field);
  to_lower(result);
  return result == "true" || result == "1";
}
// ~~ stormy::common::yaml::util
}}}
