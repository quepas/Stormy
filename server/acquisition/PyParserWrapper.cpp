#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "PyFunction.h"
#include "PyObjectMapper.h"
#include "TypeConfiguration.h"
#include "Utils.h"
#include "MeteoUtils.h"

using namespace Stormy;
using namespace Meteo;

PyParserWrapper::PyParserWrapper( std::string _pyParserModuleName )
	:	pyParserModuleName(_pyParserModuleName)
{	
}

PyParserWrapper::~PyParserWrapper()
{	
}

Measurement* PyParserWrapper::parseFromURL( std::string url )
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
		TypeConfiguration* types = new TypeConfiguration("config/meteo_data_type_config.yaml");
		
		Measurement* result = new Measurement();
		for(auto it = data.begin(); it != data.end(); ++it) {
			std::string id = types -> getTypeIdByEquivalent(it -> first);
			Type* type = types -> getFullTypeById(id);
			std::string valueType = boost::to_lower_copy(type -> valueType);
			std::string value = boost::trim_copy(it -> second);

			if(value != "-") {				
				if(valueType == "number")
					result -> data[id] = MeteoUtils::extractTemperature(value);
				else if(valueType == "text")
					result -> data[id] = value;	
			}			
		}
		return result;
	} 
	else
	{
		std::cout << "No data at current URL" << std::endl;
		return nullptr;
	}			
}

Measurement* PyParserWrapper::parseFromStation( Station* station )
{	
	return parseFromURL(station -> url);
}
