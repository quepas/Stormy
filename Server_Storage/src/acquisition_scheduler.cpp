#include "acquisition_scheduler.h"

#include "../../common/util.h"

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

void Scheduler::Schedule(Setting* server)
{
	schedule(new Task(storage_database_, server), 1000, server -> interval * 1000);
}

void Scheduler::Schedule( 
	const vector<Setting*>& servers )
{
	common::Each(servers, [&](Setting* server) {
		Schedule(server);
	});
}

void Scheduler::SetDBStorage( db::Storage* _dbStorage )
{
	if(_dbStorage) storage_database_ = _dbStorage;
}
// ~~ stormy::acquisition::Scheduler
}}
