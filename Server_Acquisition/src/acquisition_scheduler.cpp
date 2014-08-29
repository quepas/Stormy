#include "acquisition_scheduler.hpp"

#include "acquisition_task.hpp"
#include "common/util.h"

#include <Poco/Logger.h>

using namespace stormy::common;
using Poco::Logger;
using std::vector;

namespace stormy {
  namespace acquisition {

Scheduler::Scheduler(PyScriptStorage& storage)
  : storage_(storage)
{

}

Scheduler::~Scheduler()
{

}

void Scheduler::Schedule(StationSetting station)
{
  auto parse_script = storage_.Fetch(station.parse_script);
  if (parse_script != nullptr) {
    schedule(new Task(station, parse_script), 0,
      static_cast<long>(SecondsToMiliseconds(station.update_time)));
  }
  else {
    Logger::get("acquisition/Scheduler")
      .error("Missing parse script with class: " + station.parse_script);
  }
  
}

void Scheduler::Schedule(const StationSettings& stations)
{
  for(auto it = stations.begin(); it != stations.end(); ++it) {
    Schedule(*it);
  }
}
// ~~ stormy::acquisition::Scheduler
}}
