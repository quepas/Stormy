#pragma once

#include <Poco/Logger.h>
#include <Poco/Util/TimerTask.h>

#include "../../aggregation_entity_task.h"
#include "../../DBStorage.h"
#include "../../DBAggregation.h"

namespace stormy {
  namespace aggregate {
    namespace task {

class Base : public Poco::Util::TimerTask
{
public:  
  Base(aggregation::entity::Task task_data, Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation);
  virtual ~Base();

  void run() = 0;
protected:
  aggregation::entity::Task task_entity_;
  Stormy::DBStorage* storage_;
  Stormy::DBAggregation* aggregation_;

  //std::tm task_ts; TODO: make use of this
  Poco::Logger& logger_;

  std::string prepareHeader(std::string task_type);
};
// ~~ stormy::aggregate::task::Base
}}}