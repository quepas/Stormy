#include "AcquisitionScheduler.h"

#include "../../common/Utils.h"

using namespace Stormy;
using namespace std;

AcquistionScheduler::AcquistionScheduler( 
	DBStorage* _dbStorage, const std::vector<AcquisitionServer*>& servers )
		:	dbStorage(_dbStorage)
{
	scheduleManyAcquisition(servers);
}

AcquistionScheduler::~AcquistionScheduler()
{
	Utils::forEach(acquiredServerTask, [](AcquistionTask* server) {
		delete server;
	});
}

void AcquistionScheduler::scheduleAcquisition( AcquisitionServer* server )
{
	schedule(new AcquistionTask(dbStorage, server), 0, 30000);
}

void AcquistionScheduler::scheduleManyAcquisition( 
	const vector<AcquisitionServer*>& servers )
{
	Utils::forEach(servers, [&](AcquisitionServer* server) {
		scheduleAcquisition(server);
	});
}

void AcquistionScheduler::setDBStorage( DBStorage* _dbStorage )
{
	if(_dbStorage) dbStorage = _dbStorage;
}