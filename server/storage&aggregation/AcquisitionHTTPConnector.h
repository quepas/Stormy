#pragma once

#include <string>
#include <vector>

#include "../../common/Types.h"

namespace Stormy
{
	class AcquisitionHTTPConnector
	{
		public:
			AcquisitionHTTPConnector();
			~AcquisitionHTTPConnector();

			static std::string getDataAsStringAt
				(std::string host, uint32 port, std::string resource);
			static StationPtrVector getStationsAt(
				std::string host, uint32 port);
			static MeasurementPtrVector getMeasurementsForStationAt(
				std::string host, uint32 port, std::string stationId);
			static MetricsPtrVector getMetricsAt(
				std::string host, uint32 port);
		private:

	};
}