#include "acquisition_scheduler.h"

#include "util.h"

using std::vector;

namespace stormy {
  namespace acquisition {

Scheduler::Scheduler( db::Storage* _dbStorage )
		:	storage_database_(_dbStorage)
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::Schedule(RemoteServerSetting server)
{
	schedule(new Task(storage_database_, server), 1000, static_cast<long>(server.update_time * 1000));
}

void Scheduler::Schedule(const vector<RemoteServerSetting>& servers)
{
  for (auto& entry : servers) {
    Schedule(entry);
  }
}

void Scheduler::SetDBStorage( db::Storage* _dbStorage )
{
	if(_dbStorage) storage_database_ = _dbStorage;
}
// ~~ stormy::acquisition::Scheduler
}}
