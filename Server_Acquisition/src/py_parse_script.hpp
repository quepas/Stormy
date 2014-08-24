#pragma once

#define HAVE_ROUND
#define HAVE_HYPOT
#define BOOST_PYTHON_STATIC_LIB
#include <boost/python.hpp>

#include <string>
#include <map>

namespace stormy {

class PyParseScript
{
public:
  PyParseScript(const std::string& file_path);
  ~PyParseScript();

  // call 'process' function
  std::map<std::string, std::string> operator()(const std::string& data_to_parse) const;

private:
  PyParseScript(const PyParseScript&);
  PyParseScript& operator=(const PyParseScript&);

  std::string file_path_;

  boost::python::object module_;
  boost::python::object namespace_;
};

}
// stormy::PyParseScript
