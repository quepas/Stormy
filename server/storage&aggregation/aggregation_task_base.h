#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "aggregation_entity_task.h"
#include "DBStorage.h"
#include "DBAggregation.h"

namespace stormy {
  namespace aggregation {
    namespace task {

class Base : public Poco::Util::TimerTask
{
public:  
  Base(entity::Task task_data, Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation);
  virtual ~Base();

  void run() = 0;
protected:
  entity::Task task_entity_;
  Stormy::DBStorage* storage_;
  Stormy::DBAggregation* aggregation_; 
  Poco::Logger& logger_;

  std::string PrepareHeader(std::string task_type);
};
// ~~ stormy::aggregation::task::Base
}}}