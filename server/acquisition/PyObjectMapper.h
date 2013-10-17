#pragma once

#include <map>
#include <vector>
#include <Python.h>

#include "../../common/MeteoData.h"
#include "MeteoDataTypeEquivalentCfg.h"

namespace Stormy
{
	class PyObjectMapper
	{
		public:
			static MeteoData* mapToMeteoDataWithRules(std::map<std::string, std::string> map, MeteoDataTypeEquivalentCfg* rules);

			static std::vector<PyObject*> extractItemsFromSequence(PyObject* sequence);
			static PyObject* extractKeyFromSingleDict(PyObject* dict);
			static PyObject* extractValueFromSingleDict(PyObject* dict);
			static std::map<std::string, std::string> extractDictsFromDictSequence(PyObject* dictSequence);
	};
}