#include "py_read_script.hpp"

using boost::python::exec_file;
using boost::python::import;
using boost::python::object;
using std::string;

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
}

void PyReadScript::operator()(string data_to_process)
{
  try {    
    object ignored = exec_file(file_path_.c_str(), namespace_, namespace_);
    object run = namespace_["process"];
    object result = run(data_to_process);
  }
  catch (...)
  {
    PyErr_Print();
  }  
}

// ~~ stormy::PyReadScript
}
