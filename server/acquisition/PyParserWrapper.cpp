#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>

#include "PyFunction.h"
#include "PyObjectMapper.h"
#include "EquivalentsConfig.h"
#include "Utils.h"

using namespace Stormy;

PyParserWrapper::PyParserWrapper( std::string _pyParserModuleName )
	:	pyParserModuleName(_pyParserModuleName)
{	
}

PyParserWrapper::~PyParserWrapper()
{	
}

MeteoData* PyParserWrapper::parseFromURL( std::string url )
{	
	PyObject* pArgs = PyTuple_New(1);
	PyObject* pURLValue = PyUnicode_FromString(url.c_str());
	PyTuple_SetItem(pArgs, 0, pURLValue);	
	
	PyObject* pFuncResult = PyFunction(pyParserModuleName.c_str(), "run")(pArgs);
	Py_DECREF(pURLValue);
	Py_DECREF(pArgs);

	if(pFuncResult != nullptr)
	{						
		std::map<std::string, std::string> data = 
			PyObjectMapper::extractDictsFromDictSequence(pFuncResult);
		EquivalentsConfig* equivalents = 
			new EquivalentsConfig("config/meteo_data_type_equivalents.yaml");

		// TODO: PyObjectMapper::mapToMeteoDataWithRules !
		MeteoData* result = new MeteoData();
		for(auto it = data.begin(); it != data.end(); ++it) {
			std::string key = it -> first;
			std::string value = it -> second;

			SingleMeteoData* singleData = new SingleMeteoData();
			singleData -> text = new std::string(value);
			result -> data.insert(
				std::make_pair(equivalents->getTypeByEquivalent(key), singleData));
		}
		return result;
	} 
	else
	{
		std::cout << "No data at current URL" << std::endl;
		return nullptr;
	}			
}
