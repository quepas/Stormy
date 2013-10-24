#pragma once

#include <map>
#include <vector>
#include <string>

#include <Poco/Timestamp.h>
#include <boost/algorithm/string.hpp>

#include "MeteoStation.h"

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

	union SingleMeteoData
	{		
		std::string* text;
		double number;
		int integer;		
	};

	struct MeteoData
	{
		std::map<TYPE, SingleMeteoData*> data;

		bool contains(TYPE type) {
			return data.find(type) != data.end();
		}

		MeteoData()
			:	timestamp(0), windSpeedDirection("")
		{
			airTemperature = airHumidity = atmosphericPressure =
			windSpeed = windSpeedAngle = windSpeedMedium = momentDrop =
			perceptibleTemperature = uvRadiation = solarRadiation =
			dewPoint = 0;		 
		}

		// POD
		MeteoStation* station;

		Poco::Int64 timestamp;
		double airTemperature;
		double airHumidity;
		double atmosphericPressure;
		double windSpeed;
		double windSpeedAngle;
		std::string windSpeedDirection;
		double windSpeedMedium;
		double momentDrop;
		double perceptibleTemperature;
		double uvRadiation;
		double solarRadiation;
		double dewPoint;
	};

	struct MeteoDataType
	{
		TYPE type;
		VALUE_TYPE valueType;
		UNIT_TYPE unitType;
		std::vector<std::string> equivalents;

		static std::string getStringType(TYPE type)
		{
			switch(type)
			{
				case AIR_TEMPERATURE:
					return "AIR_TEMPERATURE";
				case PERCEPTIBLE_TEMPERATURE:
					return "PERCEPTIBLE_TEMPERATURE";
				case AIR_HUMIDITY:
					return "AIR_HUMIDITY";
				case ATMOSPHERIC_PRESSURE:
					return "ATMOSPHERIC_PRESSURE";
				case WIND_SPEED:
					return "WIND_SPEED";
				case WIND_MEDIUM_SPEED:
					return "WIND_MEDIUM_SPEED";
				case MOMENT_DROP:
					return "MOMENT_DROP";
				case UV_RADIATION:
					return "UV_RADIATION";
				case SOLAR_RADIATION:
					return "SOLAR_RADIATION";
				case DEW_POINT:
					return "DEW_POINT";
				default:
					return "T_UNKNOWN";
			}
		}

		static TYPE getTypeFromString(std::string text)
		{
			boost::to_upper(text);
			if(text == "AIR_TEMPERATURE")
				return AIR_TEMPERATURE;
			if(text == "PERCEPTIBLE_TEMPERATURE")
				return PERCEPTIBLE_TEMPERATURE;
			if(text == "AIR_HUMIDITY")
				return AIR_HUMIDITY;
			if(text == "ATMOSPHERIC_PRESSURE")
				return ATMOSPHERIC_PRESSURE;
			if(text == "WIND_SPEED")
				return WIND_SPEED;
			if(text == "WIND_MEDIUM_SPEED")
				return WIND_MEDIUM_SPEED;
			if(text == "MOMENT_DROP")
				return MOMENT_DROP;
			if(text == "UV_RADIATION")
				return UV_RADIATION;
			if(text == "SOLAR_RADIATION")
				return SOLAR_RADIATION;
			if(text == "DEW_POINT")
				return DEW_POINT;
			return T_UNKNOWN;
		}

		static VALUE_TYPE getValueTypeFromString(std::string valueType)
		{
			boost::to_upper(valueType);
			if(valueType == "NUMBER")
				return NUMBER;
			if(valueType == "TEXT")
				return TEXT;
			if(valueType == "INTEGER")
				return INTEGER;
			if(valueType == "DATE")
				return DATE;
			if(valueType == "TIME")
				return TIME;				
			return VT_UNKNOWN;					
		}

		static UNIT_TYPE getUnitTypeFromString(std::string unitType)
		{
			boost::to_upper(unitType);
			if(unitType == "CELSIUS")
				return CELSIUS;
			return UT_UNKNOWN;
		}
	};
}