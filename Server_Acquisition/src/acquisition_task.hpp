#pragma once

#include <Poco/Util/TimerTask.h>
#include <Poco/Logger.h>

#include "entity_station.h"
#include "py_parse_script.hpp"
#include "db_mongo_handler.hpp"
#include "settings.hpp"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
  Task(StationSetting station, PyParseScript* script);
  ~Task();

  void run() override;
private:
  StationSetting station_;
  db::MongoHandler& database_handler_;
  PyParseScript* script_;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}
