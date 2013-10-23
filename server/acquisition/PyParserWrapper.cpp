#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>

#include "PyFunction.h"
#include "PyObjectMapper.h"
#include "MeteoDataTypeEquivalentCfg.h"

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
		MeteoDataTypeEquivalentCfg* equivalents = new 
			MeteoDataTypeEquivalentCfg("config/meteo_data_type_equivalents.yaml");

		MeteoData* result = new MeteoData();
		for(auto it = data.begin(); it != data.end(); ++it) {
			SingleMeteoData* singleData = new SingleMeteoData();
			singleData -> text = new std::string(it -> second);
			result -> data.insert(
				std::make_pair(equivalents->getTypeByEquivalent(it -> first), singleData));			
		}
		return result;
	} 
	else
	{
		std::cout << "No data at current URL" << std::endl;
		return nullptr;
	}			
}

MeteoData* PyParserWrapper::parseFromText( std::wstring text )
{
	return nullptr;
}
