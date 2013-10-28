#pragma once

#include "MeteoData.h"

namespace Stormy
{
	class JSONUtils
	{
		public:
			std::string prepareJSONForStation(Meteo::Station* station);
			std::string prepareJSONForStations(const std::vector<Meteo::Station*>& stations);

			std::string prepareJSONForMeasurement(Meteo::Measurement* measurement);
			std::string prepareJSONForMeasurements(const std::vector<Meteo::Measurement*>& measurements);

		private:
	};
}