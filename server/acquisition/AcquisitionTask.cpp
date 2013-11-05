#include "AcquisitionTask.h"

#include <iostream>
#include <Python.h>

using namespace Stormy;
using namespace Meteo;

AcquisitionTask::AcquisitionTask(Station* _station)
	:	station(_station),
		pyParser(new PyParserWrapper(station -> parserClass)),
		dbHandler(new MongoDBHandler("localhost"))
{

}

AcquisitionTask::~AcquisitionTask()
{

}

void AcquisitionTask::run()
{
	std::cout << "Acquisition weather from "
		<< station -> stationId << " (Every "
		<< station -> refreshTime << " minutes.)" << std::endl;

	dbHandler -> insertMeteoData(pyParser -> parseFromStation(station));
}
