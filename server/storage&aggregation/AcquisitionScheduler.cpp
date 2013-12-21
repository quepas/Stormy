#include "AcquisitionScheduler.h"

#include "../../common/Utils.h"

using namespace Stormy;
using namespace std;

AcquistionScheduler::AcquistionScheduler( DBStorage* _dbStorage )
		:	dbStorage(_dbStorage)
{

}

AcquistionScheduler::~AcquistionScheduler()
{
	Utils::forEach(acquiredServerTask, [](stormy::acquisition::Task* server) {
		delete server;
	});
}

void AcquistionScheduler::scheduleAcquisition( stormy::acquisition::Setting* server )
{
	schedule(new stormy::acquisition::Task(dbStorage, server), 10000, server -> interval * 1000);
}

void AcquistionScheduler::scheduleManyAcquisition( 
	const vector<stormy::acquisition::Setting*>& servers )
{
	Utils::forEach(servers, [&](stormy::acquisition::Setting* server) {
		scheduleAcquisition(server);
	});
}

void AcquistionScheduler::setDBStorage( DBStorage* _dbStorage )
{
	if(_dbStorage) dbStorage = _dbStorage;
}
