#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>
#include <Poco/Timestamp.h>
#include "../entity/Task.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class BaseTask : public Poco::Util::TimerTask
{
public:
  explicit BaseTask(entity::Task task_data);
  virtual ~BaseTask();

  void run() override = 0;
protected:
  entity::Task task_entity_;
  Poco::Timestamp task_ts;
  Poco::Logger& logger_;
};
// ~~ stormy::aggregate::task::BaseTask
}}}