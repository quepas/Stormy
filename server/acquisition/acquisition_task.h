#pragma once

#include <Poco/Util/TimerTask.h>
#include <Poco/Logger.h>

#include "MeteoData.h"
#include "py_parser.h"
#include "MongoDBHandler.h"

namespace stormy {
  namespace acquisition {

class Task : public Poco::Util::TimerTask
{
public:
  Task(Stormy::Meteo::Station station);
  ~Task();

  void run() override;
private:
  Stormy::Meteo::Station station_;
  py::Parser* py_parser_;
  Stormy::MongoDBHandler& database_handler_;
  Poco::Logger& logger_;
};
// ~~ stormy::acquisition::Task
}}
