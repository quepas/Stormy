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
