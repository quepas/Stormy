#include "acquisition_scheduler.h"

#include "acquisition_task.h"
#include "acquisition_util.h"

using namespace stormy::common;
using std::vector;

namespace stormy {
  namespace acquisition {

Scheduler::Scheduler()
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::Schedule(entity::Station station)
{
	schedule(new Task(station), 0,
		util::MinutesToMiliseconds(station.refresh_time));
}

void Scheduler::Schedule(const vector<entity::Station>& stations)
{
	for(auto it = stations.begin(); it != stations.end(); ++it) {
		Schedule(*it);
	}
}
// ~~ stormy::acquisition::Scheduler
}}
