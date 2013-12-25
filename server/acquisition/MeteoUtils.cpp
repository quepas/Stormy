#include "MeteoUtils.h"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include "../../common/util.h"
#include "acquisition_constant.h"

using namespace Stormy;

double MeteoUtils::extractTemperature( std::string text )
{
	std::vector<std::string> splited;
	boost::trim(text);
	boost::replace_all(text, ",", ".");
	boost::split(splited, text, boost::is_any_of(" "));

	for(auto it = splited.begin(); it != splited.end(); ++it) {
		std::string value = *it;
		if(stormy::common::IsNumeric(value)) {
			return stormy::common::ToDouble(value);
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

std::string MeteoUtils::getMeteoDb()
{
	return stormy::acquisition::constant::mongoDb + "." + stormy::acquisition::constant::mongoMeteo;
}

std::string MeteoUtils::getStationDb()
{
	return stormy::acquisition::constant::mongoDb + "." + stormy::acquisition::constant::mongoStation;
}

std::string Stormy::MeteoUtils::getTypeDb()
{
	return stormy::acquisition::constant::mongoDb + "." + stormy::acquisition::constant::mongoType;
}
