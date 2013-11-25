#include "AggregationTask.h"

#include <iostream>
#include <Poco/Stopwatch.h>

using namespace Stormy;
using namespace Poco;
using namespace std;

AggregationTask::AggregationTask( DBStorage* _storage )
	:	storage(_storage)
{

}

AggregationTask::~AggregationTask()
{

}

void AggregationTask::run()
{
	Stopwatch stopwatch;
	stopwatch.start();

	cout << "[AggregationTask] Did work in: " 
		<< stopwatch.elapsed() / 1000.0f << " seconds."  << endl;

	stopwatch.stop();
}
