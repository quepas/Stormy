#include "PyObjectMapper.h"

#include <iostream>
#include <string>

#include "MeteoUtils.h"

using namespace Stormy;
using namespace Meteo;

std::vector<PyObject*> PyObjectMapper::extractItemsFromSequence( PyObject* sequence )
{
	auto result = std::vector<PyObject*>();

	if(PySequence_Check(sequence))
	{
		unsigned int size = PySequence_Size(sequence);
		for(unsigned int i = 0; i < size; ++i)
		{
			result.push_back(PySequence_GetItem(sequence, i));
		}
	}
	return result;
}

PyObject* PyObjectMapper::extractKeyFromSingleDict( PyObject* dict )
{
	PyObject* result = nullptr;
	if(PyDict_Check(dict) && PyDict_Size(dict) == 1) {
		return PySequence_GetItem(PyDict_Keys(dict), 0);
	}
	return result;
}

PyObject* PyObjectMapper::extractValueFromSingleDict( PyObject* dict )
{
	PyObject* result = nullptr;
	if(PyDict_Check(dict) && PyDict_Size(dict) == 1) {
		return PySequence_GetItem(PyDict_Values(dict), 0);
	}
	return result;
}

std::map<std::string, std::string> PyObjectMapper::extractDictsFromDictSequence( PyObject* dictSequence )
{
	auto result = std::map<std::string, std::string>();
	auto itemsSequence = extractItemsFromSequence(dictSequence);

	if(itemsSequence.size() > 0)
	{
		for(auto it = itemsSequence.begin(); it != itemsSequence.end(); ++it)
		{
			PyObject* pyKey = extractKeyFromSingleDict(*it);
			PyObject* pyValue = extractValueFromSingleDict(*it);

			if(pyKey != nullptr && pyValue != nullptr)
			{
				std::string key = PyUnicode_AsUTF8(pyKey);				
				std::string value = PyUnicode_AsUTF8(pyValue);				
				result.insert(std::make_pair(key, value));
			}					
		}
	}
	return result;
}

Measurement* PyObjectMapper::mapToMeteoMeasurementWithRules( std::map<std::string, std::string>& map, TypeConfiguration* rules )
{
	Measurement* result = new Measurement();
	for(auto it = map.begin(); it != map.end(); ++it)
	{
		std::string key = it -> first;
		std::string value = it -> second;		
	}
	return result;
}
