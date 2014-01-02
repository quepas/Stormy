#include "acquisition_task.h"

#include <Python.h>

using namespace stormy::common;
using Poco::Logger;

namespace stormy {
  namespace acquisition {

Task::Task(entity::Station station)
  : logger_(Logger::get("acquisition/Task")),
    station_(station),
    py_parser_(new py::Parser(station_.parser_class)),
    database_handler_(db::MongoHandler::get())
{

}

Task::~Task()
{
  delete py_parser_;
}

void Task::run()
{
  logger_.information("[acquisition/Task] Acquire weather from " + station_.uid);
  database_handler_.insertMeteoData(
    py_parser_ -> ParseFromStation(station_));
}
// ~~ stormy::acquisition::Task
}}
