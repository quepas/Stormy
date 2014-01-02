#include "py_mapper.h"

#include "acquisition_util.h"

#include <cstdint>

using std::string;
using std::map;
using std::make_pair;
using std::vector;

namespace stormy {
  namespace py {
    namespace mapper {

vector<PyObject*> ItemsFromSequence(PyObject* sequence)
{
  auto result = vector<PyObject*>();
  if (PySequence_Check(sequence)) {
    uint16_t size = PySequence_Size(sequence);
    for (uint16_t i = 0; i < size; ++i) {
      result.push_back(PySequence_GetItem(sequence, i));
    }
  }
  return result;
}

PyObject* PairKey(PyObject* pair)
{
  PyObject* result = nullptr;
  if (PyDict_Check(pair) && PyDict_Size(pair) == 1) {
    PyObject* dict_keys = PyDict_Keys(pair);
    result = PySequence_GetItem(dict_keys, 0);
    Py_DECREF(dict_keys);
  }
  return result;
}

PyObject* PairValue(PyObject* pair)
{
  PyObject* result = nullptr;
  if (PyDict_Check(pair) && PyDict_Size(pair) == 1) {
    PyObject* dict_value = PyDict_Values(pair);
    result = PySequence_GetItem(dict_value, 0);
    Py_DECREF(dict_value);
  }
  return result;
}

map<string, string> PairsFromSequence(PyObject* dictSequence)
{
  auto result = map<string, string>();
  auto itemsSequence = ItemsFromSequence(dictSequence);
  
  if (itemsSequence.size() > 0)
  {
    for (auto it = itemsSequence.begin(); it != itemsSequence.end(); ++it)
    {      
      PyObject* pyKey = PairKey(*it);
      PyObject* pyValue = PairValue(*it);
      
      if (pyKey != nullptr && pyValue != nullptr)
      {
        string key(PyUnicode_AsUTF8(pyKey));
        string value(PyUnicode_AsUTF8(pyValue));
        result.insert(make_pair(key, value));
      }
      Py_DECREF(pyKey);
      Py_DECREF(pyValue);      
    }
  }

  for (auto it = itemsSequence.begin(); it != itemsSequence.end(); ++it) {
    Py_DECREF(*it);
  }
  return result;
}
// ~~ stormy::py::mapper
}}}
