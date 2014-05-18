#pragma once

#include <Poco/Logger.h>

namespace stormy {
  namespace common {
    namespace py {

#define PY_EXECUTOR_INIT() stormy::common::py::Executor::Instance();

class Executor
{
public:
  static Executor& Instance() {
    static Executor instance;
    return instance;
  }
private:
  Executor();
  Executor(const Executor&) 
    : logger_(Poco::Logger::get("py/Executor")) {}
  Executor& operator=(const Executor&) {}
  ~Executor();
  
  bool Init();

  Poco::Logger& logger_;
};
// ~~ stormy::common::py::Executor
}}}
