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

			static std::string prepareJSONForAvailableType(Meteo::Type* type);
			static std::string prepareJSONForAvailableTypes(const std::vector<Meteo::Type*>& type);

			static std::string prepareJSONForSingleMeasurement(Meteo::Measurement* measurement);

		private:
			static std::string wrapAsJSONString(std::string label);
	};
}