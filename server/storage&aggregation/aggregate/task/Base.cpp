#include "Base.h"

using Poco::Logger;

namespace stormy {
  namespace aggregate {
    namespace task {

Base::Base(entity::Task task_data)
  : logger_(Logger::get("aggregation")),
    task_entity_(task_data)
{
}

Base::~Base()
{

}
// ~~ stormy::aggregate::task::Base
}}}