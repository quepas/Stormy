#pragma once

#include <Poco/Util/TimerTask.h>

#include "AcquisitionServer.h"
#include "DBStorage.h"

namespace Stormy
{
	class AcquistionTask : public Poco::Util::TimerTask
	{
		public:
			AcquistionTask(DBStorage* _dbStorage, AcquisitionServer* _server);
			~AcquistionTask();

			void run();
		private:
			AcquisitionServer* server;
			DBStorage* dbStorage;
	};
}