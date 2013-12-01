#include "BaseTask.h"

using Poco::Logger;

namespace stormy {
  namespace aggregate {
    namespace task {

BaseTask::BaseTask(entity::Task task_data)
  : logger_(Logger::get("aggregation")),
    task_entity_(task_data)
{
}

BaseTask::~BaseTask()
{

}
// ~~ stormy::aggregate::task::BaseTask
}}}