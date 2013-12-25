#include "py_mapper.h"

#include <cstdint>

#include "MeteoUtils.h"

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
	if (PySequence_Check(sequence))	{
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
		return PySequence_GetItem(PyDict_Keys(pair), 0);
	}
	return result;
}

PyObject* PairValue(PyObject* pair)
{
	PyObject* result = nullptr;
	if (PyDict_Check(pair) && PyDict_Size(pair) == 1) {
		return PySequence_GetItem(PyDict_Values(pair), 0);
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
				string key = PyUnicode_AsUTF8(pyKey);
				string value = PyUnicode_AsUTF8(pyValue);
				result.insert(make_pair(key, value));
			}
		}
	}
	return result;
}

Stormy::Meteo::Measurement* ToMeasurementWithMetrics(map<string, string>& map, Stormy::Meteo::TypeConfiguration* rules)
{
	Stormy::Meteo::Measurement* result = new Stormy::Meteo::Measurement();
	for(auto it = map.begin(); it != map.end(); ++it)
	{
		string key = it -> first;
		string value = it -> second;
	}
	return result;
}
// ~~ stormy::py::mapper
}}}
