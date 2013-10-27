#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "MeteoData.h"

namespace Stormy
{
	class AcquisitionScheduler : public Poco::Util::Timer
	{
		public:
			AcquisitionScheduler();
			~AcquisitionScheduler();

			void scheduleAcquisition(Meteo::Station* station);
			void scheduleManyAcquisition(std::vector<Meteo::Station*> stations);
		private:
			std::vector<Meteo::Station*> acquiredStations;
	};
}