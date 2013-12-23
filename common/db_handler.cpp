#include "db_handler.h"

using std::string;

namespace stormy {
  namespace common {
    namespace db {
      
Handler::Handler(string database_name)
  : logger_(Poco::Logger::get("db/handler")),
    database_name_(database_name),
    connected_(false)
{
  
}

Handler::~Handler()
{

}

bool Handler::ValidateConnection()
{
  if (!connected_) {   
    logger_.error("[db/handler/:" + database_name_ + "] Not connected.");
  }
  return connected_;
}
// ~~ stormy::common::db::Handler
}}}
