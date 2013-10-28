#include "Utils.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <Poco/MD5Engine.h>

using namespace Stormy;
using Poco::MD5Engine;

double Utils::stringToDouble( std::string number )
{
	double result;
	try {
		result = boost::lexical_cast<double>(number);
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

std::string Utils::md5( std::string text )
{
	MD5Engine md5Engine;
	md5Engine.update(text);
	return MD5Engine::digestToHex(md5Engine.digest());	
}

bool Utils::checkIfStandardDate( std::string date )
{
	return checkTextWithRegex(date, "[0-9]{4}-[0-9]{2}-[0-9]{2}");
}

bool Utils::checkIfStandardTime( std::string time )
{
	return checkTextWithRegex(time, "[0-9]{1, 2}:[0-9]{1, 2}");
}

bool Utils::checkTextWithRegex( std::string text, std::string regex )
{
	boost::regex regexPattern(regex);
	return boost::regex_match(text, regexPattern);
}
