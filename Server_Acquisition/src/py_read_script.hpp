#ifdef STORMY_LITE
#pragma once

#define HAVE_ROUND
#define HAVE_HYPOT
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

private:
  PyReadScript(const PyReadScript&);
  PyReadScript& operator=(const PyReadScript&);

  std::string file_path_;

  boost::python::object module_;
  boost::python::object namespace_;
};
// stormy::PyReadScript
}
#endif
