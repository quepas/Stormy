#pragma once

#include <string>
#define HAVE_ROUND
#include <Python.h>
#include <Poco/Logger.h>

namespace stormy {
  namespace common {
    namespace py {

class Function
{
public:
  Function(std::string module_name, std::string function_name);
  ~Function();

  PyObject* operator()(PyObject* py_args = nullptr);
private:
  bool Init(std::string module_name, std::string function_name);

  bool proper_init_;
  PyObject* py_function_;
  Poco::Logger& logger_;
};
// ~~ stormy::common::py::Function
}}}
