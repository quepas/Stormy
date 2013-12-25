#pragma once

#include <map>
#include <vector>
#include <Python.h>

#include "MeteoData.h"
#include "TypeConfiguration.h"

namespace Stormy
{
	class PyObjectMapper
	{
		public:
			static Meteo::Measurement* mapToMeteoMeasurementWithRules(std::map<std::string, std::string>& map, Meteo::TypeConfiguration* rules);

			static std::vector<PyObject*> extractItemsFromSequence(PyObject* sequence);
			static PyObject* extractKeyFromSingleDict(PyObject* dict);
			static PyObject* extractValueFromSingleDict(PyObject* dict);
			static std::map<std::string, std::string> extractDictsFromDictSequence(PyObject* dictSequence);
	};
}
