#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "entity_station.h"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
  Scheduler();
  ~Scheduler();

  void Schedule(common::entity::Station station);
  void Schedule(const std::vector<common::entity::Station>& stations);
};
// ~~ stormy::acquisition::Scheduler
}}
