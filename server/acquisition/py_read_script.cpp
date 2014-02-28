#include "py_read_script.hpp"

using boost::python::exec;
using boost::python::import;
using boost::python::object;

namespace stormy {

PyReadScript::PyReadScript(const std::string& file_path)
: file_path_(file_path)
{
  Py_Initialize();

  module_ = import("__main__");
  namespace_ = module_.attr("__dict__");    
}

PyReadScript::~PyReadScript()
{
  Py_Finalize();
}

void PyReadScript::operator()() const
{
  object ignored = exec("print('asd')", namespace_);
}

// ~~ stormy::PyReadScript
}
