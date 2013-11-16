#include "AcquisitionTask.h"

#include <iostream>

using namespace Stormy;
using namespace std;

AcquistionTask::AcquistionTask( DBStorage* _dbStorage, AcquisitionServer* _server)
	:	dbStorage(_dbStorage), server(_server)
{

}

AcquistionTask::~AcquistionTask()
{

}

void AcquistionTask::run()
{
	cout << "Acquisition data from server:\n\t" 
		<<	server -> toString() << endl;

	// connect & get
	// insert into db
	// dbStorage.insertMeteoData()
}
