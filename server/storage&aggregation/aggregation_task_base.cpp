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
  common::db::Setting storage_setting, 
  common::db::Setting aggregate_setting)
  : logger_(Logger::get("aggregation")),
    task_entity_(task_data),
    storage_(storage_setting),
    aggregation_(aggregate_setting)
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