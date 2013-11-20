#include "AcquisitionTask.h"

#include <iostream>
#include <Python.h>

using namespace Stormy;
using namespace Meteo;

AcquisitionTask::AcquisitionTask(StationPtr _station)
	:	station(_station),
		pyParser(new PyParserWrapper(station -> parserClass)),
		dbHandler(MongoDBHandler::get())
{
	
}

AcquisitionTask::~AcquisitionTask()
{
	delete pyParser;
}

void AcquisitionTask::run()
{
	std::cout << "Acquisition weather from "
		<< station -> stationId << " (Every "
		<< station -> refreshTime << " minutes.)" << std::endl;

	dbHandler.insertMeteoData(pyParser -> parseFromStation(station));
}
