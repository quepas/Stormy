#pragma once

#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include <string>

namespace stormy {

class PyReadScript
{
public:
  PyReadScript(const std::string& file_path);
  ~PyReadScript();

  // call 'read' function
  void operator()() const;

  PyReadScript(const PyReadScript&) = delete;
  PyReadScript& operator=(const PyReadScript&) = delete;

private:
  std::string file_path_;

  mutable boost::python::object module_;
  mutable boost::python::object namespace_;
};
// stormy::PyReadScript
}