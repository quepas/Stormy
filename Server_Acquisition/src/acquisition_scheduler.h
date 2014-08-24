#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "py_script_storage.hpp"
#include "entity_station.h"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
  Scheduler(PyScriptStorage& storage);
  ~Scheduler();

  void Schedule(common::entity::Station station);
  void Schedule(const std::vector<common::entity::Station>& stations);
private:
  PyScriptStorage& storage_;
};
// ~~ stormy::acquisition::Scheduler
}}
