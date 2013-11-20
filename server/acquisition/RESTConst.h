#pragma once

#include <string>

namespace Stormy
{
	namespace REST
	{
		struct Const
		{
			// common
			static std::string none;
			static std::string emptyJSON;

			// api
			static std::string station;
			static std::string meteo;

			// api patterns
			static std::string meteoStationIdPattern;	// api: /meteo/:stationId
			static std::string meteoStationIdTypePattern;	// api: /meteo/:stationId/:type
			static std::string meteoStationIdTimestampPattern;	// api: /meteo/:stationId/:timestamp
			static std::string stationPattern;			// api: /station
			static std::string infoPattern;				// api: /info
			static std::string infoStationPattern;		// api: /info/:stationId
			static std::string metricsPattern;			// api: /metrics
		};
	}
}