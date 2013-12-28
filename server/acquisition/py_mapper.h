#pragma once

#include <map>
#include <vector>
#include <string>
#include <Python.h>

#include "MeteoData.h"
#include "acquisition_config_metrics.h"

namespace stormy {
  namespace py {
    namespace mapper {

std::vector<PyObject*> ItemsFromSequence(PyObject* sequence);
PyObject* PairKey(PyObject* dict);
PyObject* PairValue(PyObject* dict);
std::map<std::string, std::string> PairsFromSequence(PyObject* dictSequence);
// ~~ stormy::py::mapper
}}}
