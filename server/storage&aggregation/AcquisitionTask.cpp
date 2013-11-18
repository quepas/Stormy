#include "AcquisitionTask.h"

#include <iostream>
#include <Poco/NumberFormatter.h>

#include "AcquisitionHTTPConnector.h"
#include "JSONUtils.h"

using namespace Stormy;
using namespace Poco;
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
	cout << "[AcquisitionTask] Fetch data from server:\n\t" 
		<<	server -> toString() << endl;	
	string resource = "/station";
	cout << "[AcquisitionTask] Try to reach: " << server -> host << ":" 
		<< server -> port << resource << endl;
	string responseContent = 
		AcquisitionHTTPConnector::getDataAt(server -> host, server -> port, resource);
	
	dbStorage ->insertStations(
		JSONUtils::extractStationsFromJSON(responseContent));

}
