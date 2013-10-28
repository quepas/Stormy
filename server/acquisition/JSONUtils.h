#pragma once

#include "MeteoData.h"

namespace Stormy
{
	class JSONUtils
	{
		public:
			static std::string prepareJSONForStation(Meteo::Station* station);
			static std::string prepareJSONForStations(const std::vector<Meteo::Station*>& stations);

			static std::string prepareJSONForMeasurement(Meteo::Measurement* measurement);
			static std::string prepareJSONForMeasurements(const std::vector<Meteo::Measurement*>& measurements);

		private:
			static std::string wrapAsJSONString(std::string label);
	};
}