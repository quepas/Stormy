#include "Base.h"

using Poco::Logger;

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
// ~~ stormy::aggregate::task::Base
}}}