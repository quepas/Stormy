#pragma once

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include <string>
#include <map>

namespace stormy {

class PyReadScript
{
public:
  PyReadScript(const std::string& file_path);
  ~PyReadScript();

  // call 'process' function
  std::map<std::string, std::string>
    operator()(const std::string& data_to_process);

  PyReadScript(const PyReadScript&) = delete;
  PyReadScript& operator=(const PyReadScript&) = delete;

private:
  std::string file_path_;

  boost::python::object module_;
  boost::python::object namespace_;
};
// stormy::PyReadScript
}
