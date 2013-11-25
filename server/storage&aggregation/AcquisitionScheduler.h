#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "DBStorage.h"
#include "AcquisitionServer.h"
#include "AcquisitionTask.h"

namespace Stormy
{
	class AcquistionScheduler : public Poco::Util::Timer
	{
		public:			
			AcquistionScheduler(DBStorage* _dbStorage);
			~AcquistionScheduler();

			void setDBStorage(DBStorage* dbStorage);

			void scheduleAcquisition(AcquisitionServer* server);
			void scheduleManyAcquisition(const std::vector<AcquisitionServer*>& servers);

		private:
			std::vector<AcquistionTask*> acquiredServerTask;
			DBStorage* dbStorage;
	};
}