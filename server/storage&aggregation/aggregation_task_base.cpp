#include "aggregation_task_base.h"

#include <Poco/NumberFormatter.h>

using std::string;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregation {
    namespace task {

Base::Base(
  entity::Task task_data, 
  db::Storage& storage_database, 
  db::Aggregate& aggregation_database)
  : logger_(Logger::get("aggregation/Task")),
    task_entity_(task_data),
    storage_database_(storage_database),
    aggregation_database_(aggregation_database)
{
}

Base::~Base()
{

}

string Base::PrepareHeader(string task_type)
{
  return "[aggregate/" + task_type + "#" +
    NumberFormatter::format(task_entity_.id) + "] ";
}
// ~~ stormy::aggregation::task::Base
}}}