#pragma once

#include <string>
#include <Python.h>

namespace Stormy
{
	class PyFunction
	{
		public:
			PyFunction(std::string moduleName, std::string functionName);
			~PyFunction();

			PyObject* operator()(PyObject* pyArgs = nullptr);
		private:
			bool init(std::string moduleName, std::string functionName);
			bool properInit;
			PyObject* pyFunction;
	};
}