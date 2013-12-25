#include "acquisition_util.h"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "../../common/util.h"
#include "acquisition_constant.h"

using boost::trim;
using boost::replace_all;
using boost::split;
using boost::is_any_of;
using std::string;
using std::vector;
using std::time_t;

namespace stormy {
  namespace acquisition {
    namespace util {

double ExtractTemperature(string text)
{
	vector<string> splited;
	trim(text);
	replace_all(text, ",", ".");
	split(splited, text, is_any_of(" "));

	for(auto it = splited.begin(); it != splited.end(); ++it) {
		string value = *it;
		if(common::IsNumeric(value)) {
			return common::ToDouble(value);
		}
	}
	return 0.0;
}

time_t SecondsToMiliseconds(time_t seconds)
{
	return (seconds > 0) ? seconds * 1000 : 0;
}

time_t MinutesToMiliseconds(time_t minutes)
{
	return SecondsToMiliseconds(minutes * 60);
}

string GetMeteoDb()
{
	return constant::mongoDb + "." + constant::mongoMeteo;
}

string GetStationDb()
{
	return constant::mongoDb + "." + constant::mongoStation;
}

string GetTypeDb()
{
	return constant::mongoDb + "." + constant::mongoType;
}
// ~~ stormy::acquisition::util
}}}
