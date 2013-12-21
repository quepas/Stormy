#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "DBStorage.h"
#include "acquisition_setting.h"
#include "acquisition_task.h"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
	Scheduler(Stormy::DBStorage* _dbStorage);
  ~Scheduler();

	void SetDBStorage(Stormy::DBStorage* dbStorage);

	void Schedule(Setting* server);
	void Schedule(const std::vector<Setting*>& servers);

private:
  Stormy::DBStorage* storage_database_;
};
// ~~ stormy::acquisition::Scheduler
}}
