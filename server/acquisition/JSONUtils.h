#pragma once

#include "MeteoData.h"

namespace Stormy
{
	class JSONUtils
	{
		public:
			static std::string prepareJSONForStation(StationPtr station);
			static std::string prepareJSONForStations(const StationPtrVector& stations);

			static std::string prepareJSONForMeasurement(MeasurementPtr measurement);
			static std::string prepareJSONForMeasurements(const MeasurementPtrVector& measurements);

			static std::string prepareJSONForAvailableType(TypePtr type);
			static std::string prepareJSONForAvailableTypes(const TypePtrVector& type);

			static std::string prepareJSONForSingleMeasurement(MeasurementPtr measurement);
			static std::string prepareJSONForSingleMeasurements(const MeasurementPtrVector& measurements);

      static std::string prepareJSONForInfo(const std::string& server_type, const TypePtrVector& type);

		private:
			static std::string wrapAsJSONString(std::string label);
	};
}