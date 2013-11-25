#pragma once

#include <Poco/Util/TimerTask.h>
#include "MeteoData.h"
#include "PyParserWrapper.h"
#include "MongoDBHandler.h"

namespace Stormy
{
	class AcquisitionTask : public Poco::Util::TimerTask
	{
		public:
			AcquisitionTask(Meteo::Station _station);
			~AcquisitionTask();

			void run();
		private:
			Meteo::Station station;
			PyParserWrapper* pyParser;
			MongoDBHandler& dbHandler;
	};
}