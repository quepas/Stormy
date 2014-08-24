#include "acquisition_task.h"
#include "net_util.hpp"

using Poco::Logger;
using std::string;

using namespace stormy::common;

namespace stormy {
  namespace acquisition {

Task::Task(entity::Station station, PyParseScript* script)
  : logger_(Logger::get("acquisition/Task")),
    station_(station),
    database_handler_(db::MongoHandler::get()),
    script_(script)
{
}

Task::~Task()
{

}

void Task::run()
{
  logger_.information("[acquisition/Task] Acquire weather from " + station_.uid);
  string website_content = net::FetchWebsite(station_.url);

  if (!website_content.empty()) {
    auto map = (*script_)(website_content);
    std::cout << "Acquired: " << map.size() << " itmes." << std::endl;

    /*auto data = py_parser_->ParseFromStation(station_);
    database_handler_.InsertMeasurement(data);*/
  }
}
// ~~ stormy::acquisition::Task
}}
