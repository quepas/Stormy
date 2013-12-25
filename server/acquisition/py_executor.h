#pragma once

namespace Stormy
{
	#define Py_ExecutorInit() Stormy::PyExecutor::getInstance();

	class PyExecutor
	{
		public:

			static PyExecutor& getInstance()
			{
				static PyExecutor instance;
				return instance;
			}
		private:
			PyExecutor();
			PyExecutor(const PyExecutor&) {}
			PyExecutor& operator=(const PyExecutor&) {}
			~PyExecutor();

			bool init();
	};
}