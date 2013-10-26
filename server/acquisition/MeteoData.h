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
			double refreshTime;
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
			std::string equivalents;
		};
	}

	enum TYPE
	{		
		AIR_TEMPERATURE,
		PERCEPTIBLE_TEMPERATURE,
		AIR_HUMIDITY,
		ATMOSPHERIC_PRESSURE,
		WIND_SPEED,
		WIND_DIRECTION,
		WIND_MEDIUM_SPEED,
		MOMENT_DROP,
		UV_RADIATION,
		SOLAR_RADIATION,
		DEW_POINT,
		T_UNKNOWN
	};

	enum VALUE_TYPE
	{
		NUMBER,
		TEXT,
		INTEGER,
		DATE,
		TIME,
		VT_UNKNOWN		
	};

	enum UNIT_TYPE
	{
		CELSIUS,
		UT_UNKNOWN
	};

	struct MeteoDataType
	{
		TYPE type;
		VALUE_TYPE valueType;
		UNIT_TYPE unitType;
		std::vector<std::string> equivalents;
	};
}