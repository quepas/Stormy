#pragma once

#include <Poco/Util/TimerTask.h>
#include <Poco/Logger.h>

#include "../../common/entity_station.h"
#include "py_parser.h"
#include "db_mongo_handler.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
  Task(common::entity::Station station);
  ~Task();

  void run() override;
private:
  common::entity::Station station_;
  py::Parser* py_parser_;
  db::MongoHandler& database_handler_;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}
