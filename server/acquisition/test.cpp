#include <iostream>

#include "PyParserWrapper.h"
#include "PyObjectMapper.h"
#include "PyExecutor.h"
#include "PyFunction.h"

#include <sstream>

template<typename Out, typename In>
Out lexical_cast(In in)
{
	std::stringstream ss;
	Out out;

	ss << in;
	ss >> out;

	return out;
};

int main() 
{	
	std::cout << "Acquisition Module Test" << std::endl;
	Py_ExecutorInit();	
	Stormy::PyParserWrapper* meteoBParser = new Stormy::PyParserWrapper("MeteoBParser");
	
	for(int i = 20; i < 40; ++i)
	{		
		meteoBParser -> parseFromURL(std::string("http://stacjameteo.pl/dane/index.php?stacja=") + lexical_cast<std::string>(i));
	}	

	getchar();
}