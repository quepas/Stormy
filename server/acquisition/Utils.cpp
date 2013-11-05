#include "Utils.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <Poco/MD5Engine.h>
#include <typeinfo>

using namespace Stormy;
using Poco::MD5Engine;
using boost::any;
using boost::any_cast;
using boost::regex;
using boost::smatch;
using boost::regex_search;
using boost::split;

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
	boost::regex numberRegex(Utils::numberPattern);
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
	return checkTextWithRegex(date, Utils::standardDatePattern);
}

bool Utils::checkIfStandardTime( std::string time )
{
	return checkTextWithRegex(time, Utils::standardTimePattern);
}

bool Utils::checkTextWithRegex( std::string text, std::string regex )
{
	boost::regex regexPattern(regex);
	return boost::regex_match(text, regexPattern);
}

std::string Utils::getStringFromAny( any value )
{
	if(typeid(double) == value.type()) {
		return lexical_cast<std::string>(any_cast<double>(value));
	}
	else if(typeid(std::string) == value.type()) {
		return any_cast<std::string>(value);
	}
	return "_none";
}

bool Utils::checkIfHexMD5( std::string text )
{
	return checkTextWithRegex(text, Utils::hexMD5Pattern);
}

std::string Utils::extractMD5FromText( std::string text )
{
	regex regex(Utils::hexMD5Pattern);
	smatch match;
	if(regex_search(text, match, regex)) {
		return match[0];
	}
	return "_none";
}

std::string Stormy::Utils::extractEndIdFromRestURI( std::string URI )
{
	if(URI.empty()) return "";
	if(getLast<char>(URI) == '/') URI.pop_back();
	std::vector<std::string> splitted;
	split(splitted, URI, boost::is_any_of("/"));
	return getLast<std::string>(splitted);
}

std::string Utils::numberPattern = "-?[0-9]+([.][0-9]+)?";
std::string Utils::standardDatePattern = "[0-9]{4}-[0-9]{2}-[0-9]{2}";
std::string Utils::standardTimePattern = "[0-9]{1, 2}:[0-9]{1, 2}";
std::string Utils::hexMD5Pattern = "[0-9a-f]{32}";
