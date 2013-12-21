#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "DBStorage.h"
#include "acquisition_setting.h"
#include "acquisition_task.h"

namespace Stormy
{
	class AcquistionScheduler : public Poco::Util::Timer
	{
		public:			
			AcquistionScheduler(DBStorage* _dbStorage);
			~AcquistionScheduler();

			void setDBStorage(DBStorage* dbStorage);

			void scheduleAcquisition(stormy::acquisition::Setting* server);
			void scheduleManyAcquisition(const std::vector<stormy::acquisition::Setting*>& servers);

		private:
			std::vector<stormy::acquisition::Task*> acquiredServerTask;
			DBStorage* dbStorage;
	};
}