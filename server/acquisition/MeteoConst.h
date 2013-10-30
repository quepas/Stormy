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
			static std::string id;
			static std::string reasonKey;
			static std::string reasonNoData;
			
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

			// value type
			static std::string number;
			static std::string text;

			// rest
			static std::string restMeteo;
			static std::string restStation;
		};		
	}
}