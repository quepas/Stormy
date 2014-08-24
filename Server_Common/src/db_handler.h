#pragma once

#include <string>
#include <Poco/Logger.h>

namespace stormy {
  namespace common {
    namespace db {

class Handler
{
public:
  Handler(std::string database_name);
  ~Handler();

  bool ValidateConnection();
protected:
  Poco::Logger& logger_;
  std::string database_name_;
  bool connected_;
};

}}}
// ~~ stormy::common::db::Handler
