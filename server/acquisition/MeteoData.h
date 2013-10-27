#pragma once

#include <map>
#include <vector>
#include <string>
#include <limits>

#include <Poco/Timestamp.h>
#include <boost/algorithm/string.hpp>
#include <boost/any.hpp>

namespace Stormy
{
	namespace Meteo 
	{
		struct Station
		{
			std::string id;
			std::string url;
			std::string name;
			std::string parserClass;
			long refreshTime;
		};

		struct Measurement
		{
			Poco::Timestamp timestamp;
			Station* station;
			std::map<std::string, boost::any> data;
		};		

		struct Type
		{
			std::string id;
			std::string valueType;
			std::string valueUnit;
			std::string valueFormat;
			std::vector<std::string> equivalents;
		};
	}
}