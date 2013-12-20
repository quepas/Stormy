#pragma once

#include <string>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/Timestamp.h>
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
			static MeasurementPtrVector getMeasurementsForStationNewerThanAt(
				std::string host, uint32 port, std::string stationId, 
				Poco::Timestamp timestamp);
			static MetricsPtrVector getMetricsAt(
				std::string host, uint32 port);
		private:
      static Poco::Logger& logger_;
	};
}