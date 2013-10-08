#include "PyFunction.h"

#include <iostream>

using namespace Stormy;

Stormy::PyFunction::PyFunction( std::string moduleName, std::string functionName )
	:	pyFunction(nullptr)
{
	properInit = init(moduleName, functionName);
}

Stormy::PyFunction::~PyFunction()
{

}

PyObject* Stormy::PyFunction::operator()( PyObject* pyArgs /*= nullptr*/ )
{
	if(!Py_IsInitialized())
	{
		std::cout << "Call Py_ExecutorInit() first" << std::endl;
		return nullptr;
	}

	if(properInit)
	{
		return PyObject_CallObject(pyFunction, pyArgs);		
	}
	return nullptr;
}

bool Stormy::PyFunction::init( std::string moduleName, std::string functionName )
{
	if(!Py_IsInitialized())
	{
		std::cout << "Call Py_ExecutorInit() first" << std::endl;
		return false;		
	}

	PyObject* pyModuleName = PyUnicode_FromString(moduleName.c_str());
	if(!pyModuleName)
	{
		std::cout << "Module name is wrong" << std::endl;
		return false;
	}

	PyObject* pyModuleData = PyImport_Import(pyModuleName);
	Py_DECREF(pyModuleName);
	if(!pyModuleData)
	{
		std::cout << "Couldn't import given module" << std::endl;
		return false;
	}

	pyFunction = PyObject_GetAttrString(pyModuleData, functionName.c_str());
	Py_DECREF(pyModuleData);
	if(!pyFunction || !PyCallable_Check(pyFunction))
	{
		std::cout << "Couldn't load given function" << std::endl;
		return false;
	}

	return true;
}




