#include "PyParserWrapper.h"

#include <Python.h>
#include <iostream>

using namespace Stormy;

Stormy::PyParserWrapper::PyParserWrapper( std::string _pyParserModuleName )
	:	pyParserModuleName(_pyParserModuleName)
{
	Py_Initialize();
}

Stormy::PyParserWrapper::~PyParserWrapper()
{
	Py_Finalize();
}

MeteoData* Stormy::PyParserWrapper::parseFromURL( std::string url )
{
	PyObject* pModuleName;
	PyObject* pModule;
	PyObject* pParseFromURL;
	PyObject* pArgs;
	
	pModuleName = PyUnicode_FromString(pyParserModuleName.c_str());
	if(pModuleName == nullptr)
	{
		std::cout << "Module name is wrong" << std::endl;
		return nullptr;
	}

	pModule = PyImport_Import(pModuleName);
	Py_DECREF(pModuleName);
	if(pModule == nullptr)
	{
		std::cout << "Couldn't import given module" << std::endl;
		return nullptr;
	}

	pParseFromURL = PyObject_GetAttrString(pModule, "run");
	Py_DECREF(pModule);
	if(pParseFromURL == nullptr || !PyCallable_Check(pParseFromURL))
	{
		std::cout << "Couldn't get parse method" << std::endl;
		return nullptr;
	}

	pArgs = PyTuple_New(1);
	PyObject* pURLValue = PyUnicode_FromString(url.c_str());
	PyTuple_SetItem(pArgs, 0, pURLValue);	

	PyObject* pFuncResult = PyObject_CallObject(pParseFromURL, pArgs);
	Py_DECREF(pParseFromURL);
	Py_DECREF(pURLValue);
	Py_DECREF(pArgs);

	if(pFuncResult != nullptr)
	{
		std::cout << "Method returns: " << PyLong_AsLong(pFuncResult) << std::endl;	
	} 
	else
	{
		std::cout << "No data at current URL" << std::endl;
	}	
	// !~ return data mapping here ~!
	
	return nullptr;
}

MeteoData* Stormy::PyParserWrapper::parseFromText( std::wstring text )
{
	return nullptr;
}
