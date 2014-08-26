#include "db_expiration_engine.h"

#include "db_expiration_task.h"
#include <Poco/NumberFormatter.h>

using std::time_t;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace common {
    namespace db {
      namespace expiration {

Engine::Engine(HasDataExpiration& database_handler)
  : logger_(Logger::get("expiration/Engine")),
    database_handler_(database_handler),
    expiration_task(nullptr)
{

}

Engine::~Engine()
{
  delete expiration_task;
}

void Engine::ScheduleEverySeconds(time_t seconds_interval)
{  
  expiration_task = new Task(database_handler_);
  schedule(expiration_task, 0, static_cast<long>(seconds_interval * 1000));

  logger_.information("[expiration/Engine] Scheduled for every " +
    NumberFormatter::format(seconds_interval) + " seconds.");  
}
// ~~ stormy::common::db::expiration::Engine
}}}}
