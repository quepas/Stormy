#pragma once

#include <Poco/Util/Timer.h>

#include "MeteoData.h"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
  Scheduler();
  ~Scheduler();

  void Schedule(Stormy::StationPtr station);
  void Schedule(const Stormy::StationPtrVector& stations);
private:
  Stormy::StationPtrVector acquiredStations;
};
// ~~ stormy::acquisition::Scheduler
}}
