#pragma once

#include <boost/python.hpp>

#include <string>
#include <map>

namespace stormy {

class PyParseScript
{
public:
  PyParseScript(const std::string& file_path);
  ~PyParseScript();

  std::map<std::string, std::string> operator()(const std::string& data_to_parse) const;
  const std::string& file_path() const { return file_path_; }

private:
  std::string file_path_;

  boost::python::object module_;
  boost::python::object namespace_;
};

}
// stormy::PyParseScript
