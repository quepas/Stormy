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

void AcquisitionScheduler::scheduleAcquisition( Station* station )
{	
	schedule(new AcquisitionTask(station), 0, 
		MeteoUtils::convertMinutesToMiliseconds(station -> refreshTime));	
}

void Stormy::AcquisitionScheduler::scheduleManyAcquisition( std::vector<Meteo::Station*> stations )
{
	for(auto it = stations.begin(); it != stations.end(); ++it) {
		scheduleAcquisition(*it);
	}
}
