#pragma once

#include <Poco/Util/Timer.h>

namespace stormy {
  namespace util {

class TaskScheduler : public Poco::Util::Timer
{
public:
  TaskScheduler() {}
  ~TaskScheduler() {}

  template<typename Task, typename TaskContext>
  void Schedule(TaskContext context) {
    schedule(new Task(context), context.delay, context.interval);
  }
};

}}
// ~~ stormy::util::TaskScheduler
