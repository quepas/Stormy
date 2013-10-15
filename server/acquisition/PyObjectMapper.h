#pragma once

#include <map>
#include <vector>
#include <Python.h>

#include "../../common/MeteoData.h"

namespace Stormy
{
	class PyObjectMapper
	{
		public:
			static MeteoData* mapToMeteoData(std::map<std::string, std::string> map);

			static std::vector<PyObject*> extractItemsFromSequence(PyObject* sequence);
			static PyObject* extractKeyFromSingleDict(PyObject* dict);
			static PyObject* extractValueFromSingleDict(PyObject* dict);
			static std::map<std::string, std::string> extractDictsFromDictSequence(PyObject* dictSequence);
	};
}
