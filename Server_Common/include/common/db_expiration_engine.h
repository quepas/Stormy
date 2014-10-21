#pragma once

#include "db_expiration_task.h"
#include "db_has_data_expiration.h"
/*
#include <ctime>
#include <Poco/Logger.h>
#include <Poco/Util/Timer.h>

namespace stormy {
  namespace common {
    namespace db {
      namespace expiration {

class Engine : protected Poco::Util::Timer
{
public:
  Engine(HasDataExpiration& database_handler);
  ~Engine();

  void ScheduleEverySeconds(std::time_t seconds_interval);
private:
  Poco::Logger& logger_;
  HasDataExpiration& database_handler_;
  Task* expiration_task;
};
// ~~ stormy::common::db::expiration::Engine
}}}}*/
