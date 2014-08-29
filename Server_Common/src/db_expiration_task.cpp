#include "common/db_expiration_task.h"

using Poco::Logger;

namespace stormy {
  namespace common {
    namespace db {
      namespace expiration {

Task::Task(HasDataExpiration& database_handler)
  : logger_(Logger::get("expiration/task")),
    database_handler_(database_handler)
{
  
}

Task::~Task()
{

}

void Task::run()
{
  database_handler_.ExpireData();
  logger_.information("[expiration/task] Job finished.");
}
// ~~ stormy::common::db::expiration::Task
}}}}
