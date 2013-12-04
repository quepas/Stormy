#include "Base.h"

#include <Poco/NumberFormatter.h>

using std::string;
using Poco::Logger;
using Poco::NumberFormatter;

namespace stormy {
  namespace aggregate {
    namespace task {

Base::Base(entity::Task task_data, Stormy::DBStorage* storage, Stormy::DBAggregation* aggregation)
  : logger_(Logger::get("aggregation")),
    task_entity_(task_data),
    storage_(storage),
    aggregation_(aggregation)
{
}

Base::~Base()
{

}

string Base::prepareHeader(string task_type)
{
  return "[aggregate::" + task_type + "#" +
    NumberFormatter::format(task_entity_.id) + "]";
}

// ~~ stormy::aggregate::task::Base
}}}