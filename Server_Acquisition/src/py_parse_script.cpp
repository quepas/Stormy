#include "py_parse_script.hpp"

namespace py = boost::python;
using std::cout;
using std::endl;
using std::make_pair;
using std::map;
using std::string;

namespace stormy {

PyParseScript::PyParseScript(const string& file_path)
  : file_path_(file_path)
{
  Py_Initialize();
  module_ = py::import("__main__");
  namespace_ = module_.attr("__dict__");
}

PyParseScript::~PyParseScript()
{

}

map<string, string> PyParseScript::operator()(const string& data_to_parse) const
{
  map<string, string> result_map;
  try {
    py::object ignored = py::exec_file(file_path_.c_str(), namespace_, namespace_);
    py::object run = namespace_["process"];
    py::object result = run(data_to_parse);

    py::extract<py::dict> result_as_dict(result);
    auto dict = result_as_dict();
    auto keys = dict.keys();

    for (int i = 0; i < py::len(keys); ++i){
      py::extract<string> extracted_key(keys[i]);

      if (!extracted_key.check()) {
        cout << "Invalid key." << endl;
        continue;
      }
      string key = extracted_key;
      py::extract<string> extracted_value(dict[key]);

      if (!extracted_value.check()) {
        cout << "Invalid value for key " << key << "." << endl;
        continue;
      }
      string value = extracted_value;
      result_map.insert(make_pair(key, value));
    }
  }
  catch (...) {
    PyErr_Print();
  }
  return result_map;
}

}
// ~~ stormy::PyParseScript
