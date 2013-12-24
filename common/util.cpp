#include "util.h"

#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <Poco/MD5Engine.h>
#include <Poco/NumberFormatter.h>
#include <typeinfo>

using boost::any;
using boost::any_cast;
using boost::is_any_of;
using boost::lexical_cast;
using boost::bad_lexical_cast;
using boost::regex;
using boost::regex_match;
using boost::smatch;
using boost::to_lower;
using std::string;
using std::vector;
using Poco::MD5Engine;

namespace stormy {
  namespace common {

double ToDouble(string number)
{
	double result;
	try {
		result = lexical_cast<double>(number);
	} catch(bad_lexical_cast const&) {
		result = 0.0;
	}
	return result;
}

bool IsNumeric(string text)
{
	regex numberRegex(numeric_pattern);
	return regex_match(text, numberRegex);
}

string MD5(string text)
{
	MD5Engine md5Engine;
	md5Engine.update(text);
	return MD5Engine::digestToHex(md5Engine.digest());
}

bool IsDate(string date)
{
	return IsMatch(date, date_pattern);
}

bool IsTime(string time)
{
	return IsMatch(time, time_pattern);
}

bool IsMatch(string text, string expr)
{
	regex regexPattern(expr);
	return regex_match(text, regexPattern);
}

string ToString(any value)
{
	if(typeid(double) == value.type()) {
		return lexical_cast<string>(any_cast<double>(value));
	}
	else if(typeid(string) == value.type()) {
		return any_cast<string>(value);
	}
	return "_none";
}

bool IsHexMD5(string text)
{
	return IsMatch(text, hex_md5_pattern);
}

string ExtractMD5(string text)
{
	regex regex(hex_md5_pattern);
	smatch match;
	if(regex_search(text, match, regex)) {
		return match[0];
	}
	return "_none";
}

string ExtractURIEndPathSegment(string URI)
{
	if(URI.empty()) return "";
	if(Last<char>(URI) == '/') URI.pop_back();
	vector<string> splitted;
	split(splitted, URI, is_any_of("/"));
	return Last<string>(splitted);
}

bool ToBool( std::string text )
{
	to_lower(text);
	return text == "true" || text == "1";
}

time_t LocaltimeNow()
{
  // TODO: fix this!
  return time(0) + 3600;
}
// ~~ stormy::common::util
}}
