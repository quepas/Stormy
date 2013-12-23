#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "db_storage.h"
#include "acquisition_setting.h"
#include "acquisition_task.h"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
	Scheduler(db::Storage* _dbStorage);
  ~Scheduler();

	void SetDBStorage(db::Storage* dbStorage);

	void Schedule(Setting* server);
	void Schedule(const std::vector<Setting*>& servers);

private:
  db::Storage* storage_database_;
};
// ~~ stormy::acquisition::Scheduler
}}
