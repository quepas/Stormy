#include "Utils.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace Stormy;

double Utils::stringToDouble( std::string value )
{
	double result;
	try {
		result = boost::lexical_cast<double>(value);
	} catch(boost::bad_lexical_cast const&) {
		result = 0.0;
	}
	return result;
}

bool Utils::isNumber( std::string text )
{
	boost::regex numberRegex("-?[0-9]+([.][0-9]+)?");
	return boost::regex_match(text, numberRegex);
}
