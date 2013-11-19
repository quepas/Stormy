#pragma once

#include <string>
#include <vector>

#include "../../common/data/Station.h"
#include "../../common/data/Measurement.h"

namespace Stormy
{
	class AcquisitionHTTPConnector
	{
		public:
			AcquisitionHTTPConnector();
			~AcquisitionHTTPConnector();

			static std::string getDataAsStringAt(std::string host, unsigned port, std::string resource);
			static std::vector<std::shared_ptr<Data::Station>> 
				getStationsAt(std::string host, unsigned port);
			static std::vector<std::shared_ptr<Data::Measurement>> 
				getMeasurementsForStationAt(std::string host, unsigned port, std::string stationId);
		private:

	};
}