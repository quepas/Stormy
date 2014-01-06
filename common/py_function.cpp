#include "py_function.h"

using std::string;
using Poco::Logger;

namespace stormy {
  namespace py {

Function::Function(string module_name, string function_name)
  : logger_(Logger::get("py/Function")),
    py_function_(nullptr)
{
  proper_init_ = Init(module_name, function_name);
}

Function::~Function()
{
  Py_DECREF(py_function_);
}

PyObject* Function::operator()(PyObject* py_args /*= nullptr*/)
{
  if (!Py_IsInitialized()) {
    logger_.error("[py/Function] Call PY_EXECUTOR_INIT() first.");
    Py_DECREF(py_args);
    return nullptr;
  }
  if (proper_init_) {
    return PyObject_CallObject(py_function_, py_args);
  }
  Py_DECREF(py_args);
  return nullptr;
}

bool Function::Init(string module_name, string function_name)
{
  if (!Py_IsInitialized()) {
    logger_.error("[py/Function] Call Py_ExecutorInit() first.");
    return false;
  }
  PyObject* py_module_name = PyUnicode_FromString(module_name.c_str());
  if (!py_module_name) {
    logger_.error("[py/Function] Wrong module name.");
    Py_DECREF(py_module_name);
    return false;
  }
  PyObject* py_module_data = PyImport_Import(py_module_name);
  Py_DECREF(py_module_name);
  if (!py_module_data) {
    logger_.error("[py/Function] Couldn't import given module.");
    Py_DECREF(py_module_name);
    return false;
  }
  py_function_ = PyObject_GetAttrString(py_module_data, function_name.c_str());
  Py_DECREF(py_module_data);
  if (!py_function_ || !PyCallable_Check(py_function_)) {
    logger_.error("[py/Function] Couldn't import given function.");
    return false;
  }
  return true;
}
// ~~ stormy::py::Function
}}
