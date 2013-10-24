#include "Utils.h"

#include <boost/lexical_cast.hpp>

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
