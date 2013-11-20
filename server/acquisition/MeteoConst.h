#pragma once

#include <string>

namespace Stormy
{
	namespace Meteo
	{
		struct Const
		{
			// common
			static std::string mongoId;
			static std::string mongoDb;
			static std::string mongoMeteo;
			static std::string mongoStation;
			static std::string mongoType;
			static std::string id;
			static std::string code;
			static std::string reasonKey;
			static std::string reasonNoData;
			static std::string data;

			// station
			static std::string stationId;
			static std::string stationIdPrefix;
			static std::string url;
			static std::string name;
			static std::string parserClass;
			static std::string refreshTime;

			// measurement
			static std::string date;
			static std::string time;
			static std::string timestamp;

			// value type
			static std::string number;
			static std::string text;
			static std::string type;
			static std::string unit;
			static std::string format;
			static std::string equivalents;
			static std::string isMeteo;

			// rest
			static std::string restMeteo;
			static std::string restStation;
		};
	}
}