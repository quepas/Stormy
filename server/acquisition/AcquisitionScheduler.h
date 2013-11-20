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

			void scheduleAcquisition(StationPtr station);
			void scheduleManyAcquisition(const StationPtrVector& stations);
		private:
			StationPtrVector acquiredStations;
	};
}