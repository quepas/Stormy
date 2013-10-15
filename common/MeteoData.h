#pragma once

#include <map>
#include <vector>

namespace Stormy
{
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
		DEW_POINT
	};

	union SingleMeteoData
	{
		char* s;
		double d;
		int i;
	};

	struct MeteoData
	{
		std::map<TYPE, SingleMeteoData> data;
	};

	struct MeteoDataType
	{
		TYPE type;
		std::vector<std::string> equivalents;
	};
}