#include "acquisition_util.hpp"

#include <vector>
#include <Poco/NumberParser.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

using boost::trim;
using boost::replace_all;
using boost::regex;
using boost::smatch;
using boost::regex_search;
using std::string;
using std::vector;
using std::time_t;
using Poco::NumberParser;

namespace stormy {
  namespace acquisition {
    namespace util {

bool TryExtractFirstNumeric(string text, double& out_value)
{
  vector<string> splited;
  trim(text);
  replace_all(text, ",", ".");
  regex regex("-?[0-9]+([.][0-9]+)?");
  smatch match;
  if(regex_search(text, match, regex)) {
    out_value = NumberParser::parseFloat(match[0]);
    return true;
  }
  out_value = 0.0;
  return false;
}

time_t SecondsToMiliseconds(time_t seconds)
{
  return (seconds > 0) ? seconds * 1000 : 0;
}

time_t MinutesToMiliseconds(time_t minutes)
{
  return SecondsToMiliseconds(minutes * 60);
}
// ~~ stormy::acquisition::util
}}}
