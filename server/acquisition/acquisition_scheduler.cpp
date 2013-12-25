#include "acquisition_scheduler.h"

#include "acquisition_task.h"
#include "MeteoUtils.h"

namespace stormy {
  namespace acquisition {

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::Schedule(Stormy::StationPtr station)
{
	schedule(new Task(*(station.get())), 0,
		Stormy::MeteoUtils::convertMinutesToMiliseconds(station -> refreshTime));
}

void Scheduler::Schedule(const Stormy::StationPtrVector& stations)
{
	for(auto it = stations.begin(); it != stations.end(); ++it) {
		Schedule(*it);
	}
}
// ~~ stormy::acquisition::Scheduler
}}
