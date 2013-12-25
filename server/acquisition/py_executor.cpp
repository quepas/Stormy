#include "PyExecutor.h"

#include <Python.h>
#include <iostream>

Stormy::PyExecutor::PyExecutor()
{
	if(!init())
	{
		if(Py_IsInitialized())
			std::cout << "PyExecutor already initialized" << std::endl;
		else
			std::cout << "Couldn't init PyExecutor" << std::endl;
	}
}

Stormy::PyExecutor::~PyExecutor()
{
	Py_Finalize();
}

bool Stormy::PyExecutor::init()
{
	if(!Py_IsInitialized())
	{
		Py_Initialize();
		return true;
	}
	return false;
}
