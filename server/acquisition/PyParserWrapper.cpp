#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>

#include "PyFunction.h"

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
		// !~ return data mapping here ~!
		std::cout << "Check: " << PySequence_Check(pFuncResult) << std::endl;
		std::cout << "Size: " << PySequence_Size(pFuncResult) << std::endl;		
	} 
	else
	{
		std::cout << "No data at current URL" << std::endl;
	}		
	
	return nullptr;
}

MeteoData* PyParserWrapper::parseFromText( std::wstring text )
{
	return nullptr;
}
