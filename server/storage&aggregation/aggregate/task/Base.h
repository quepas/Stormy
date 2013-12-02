#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>
#include "../entity/Task.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class Base : public Poco::Util::TimerTask
{
public:
  explicit Base(entity::Task task_data);
  virtual ~Base();

  void run() = 0;
protected:
  entity::Task task_entity_;
  //std::tm task_ts; TODO: make use of this
  Poco::Logger& logger_;
};
// ~~ stormy::aggregate::task::Base
}}}