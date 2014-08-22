#include "py_executor.h"

#define HAVE_ROUND
#include <Python.h>

using Poco::Logger;

namespace stormy {
  namespace common {
    namespace py {

Executor::Executor()
  : logger_(Logger::get("py/Executor"))
{
  if (!Init()) {
    if(Py_IsInitialized())
      logger_.warning("[py/Executor] Already initialized.");
    else
      logger_.error("[py/Executor] Couldn't initialize.");
  }
}

Executor::~Executor()
{
  Py_Finalize();
}

bool Executor::Init()
{
  if (!Py_IsInitialized()) {
    Py_Initialize();
    return true;
  }
  return false;
}
// ~~ stormy::common::py::Executor
}}}
