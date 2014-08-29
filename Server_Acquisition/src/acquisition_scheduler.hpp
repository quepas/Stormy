#pragma once

#include <vector>
#include <Poco/Util/Timer.h>

#include "py_script_storage.hpp"
#include "common/entity_station.h"
#include "settings.hpp"

namespace stormy {
  namespace acquisition {

class Scheduler : public Poco::Util::Timer
{
public:
  Scheduler(PyScriptStorage& storage);
  ~Scheduler();

  void Schedule(StationSetting station);
  void Schedule(const StationSettings& stations);
private:
  PyScriptStorage& storage_;
};
// ~~ stormy::acquisition::Scheduler
}}
