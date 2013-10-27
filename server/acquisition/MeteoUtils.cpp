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
	boost::replace_all(text, ",", ".");
	boost::split(splited, text, boost::is_any_of(" "));	

	for(auto it = splited.begin(); it != splited.end(); ++it) {
		std::string value = *it;
		if(Utils::isNumber(value)) {
			return Utils::stringToDouble(value);
		}
	}
	return 0.0;
}

long Stormy::MeteoUtils::convertSecondsToMiliseconds( long seconds )
{
	return (seconds > 0) ? seconds * 1000 : 0;
}

long MeteoUtils::convertMinutesToMiliseconds( long minutes )
{
	return convertSecondsToMiliseconds(minutes * 60);
}


