#include "acquisition_task.h"

#include <Python.h>

using Poco::Logger;

namespace stormy {
  namespace acquisition {

Task::Task(Stormy::Meteo::Station station)
	:	logger_(Logger::get("acquisition/Task")),
    station_(station),
		py_parser_(new py::Parser(station_.parserClass)),
		database_handler_(Stormy::MongoDBHandler::get())
{
	
}

Task::~Task()
{
	delete py_parser_;
}

void Task::run()
{
	logger_.information("[acquisition/Task] Acquire weather from " + station_.stationId);
	database_handler_.insertMeteoData(
    py_parser_ -> ParseFromStation(station_));
}
// ~~ stormy::acquisition::Task
}}
