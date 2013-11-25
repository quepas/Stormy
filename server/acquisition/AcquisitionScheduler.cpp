#include "AcquisitionScheduler.h"

#include <string>
#include "AcquisitionTask.h"
#include "MeteoUtils.h"

using namespace Stormy;
using namespace Meteo;

AcquisitionScheduler::AcquisitionScheduler()
{

}

AcquisitionScheduler::~AcquisitionScheduler()
{

}

void AcquisitionScheduler::scheduleAcquisition( StationPtr station )
{
	schedule(new AcquisitionTask(*(station.get())), 0,
		MeteoUtils::convertMinutesToMiliseconds(station -> refreshTime));
}

void Stormy::AcquisitionScheduler::scheduleManyAcquisition( const StationPtrVector& stations )
{
	for(auto it = stations.begin(); it != stations.end(); ++it) {
		scheduleAcquisition(*it);
	}
}
