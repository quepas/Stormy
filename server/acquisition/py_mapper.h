#pragma once

#include <map>
#include <vector>
#include <string>
#include <Python.h>

#include "MeteoData.h"
#include "TypeConfiguration.h"

namespace stormy {
  namespace py {
    namespace mapper {

Stormy::Meteo::Measurement* ToMeasurementWithMetrics(
  std::map<std::string, std::string>& map, 
  Stormy::Meteo::TypeConfiguration* rules);

std::vector<PyObject*> ItemsFromSequence(PyObject* sequence);
PyObject* PairKey(PyObject* dict);
PyObject* PairValue(PyObject* dict);
std::map<std::string, std::string> PairsFromSequence(PyObject* dictSequence);
// ~~ stormy::py::mapper
}}}
