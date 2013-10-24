#include "MeteoUtils.h"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "Utils.h"

using namespace Stormy;

double MeteoUtils::extractTemperature( std::string text )
{
	std::vector<std::string> splited;
	boost::trim(text);
	boost::split(splited, text, boost::is_any_of(" "));	

	for(auto it = splited.begin(); it != splited.end(); ++it) {
		std::string value = *it;
		if(Utils::isNumber(value)) {
			return Utils::stringToDouble(value);
		}
	}
	return 0.0;
}
