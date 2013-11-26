#include "AggregationTask.h"

#include <iostream>
#include <Poco/Stopwatch.h>

using namespace Stormy;
using namespace Poco;
using namespace std;

AggregationTask::AggregationTask( DBAggregation* _aggregation, AggregationSetting _setting, uint32 _taskId )
	:	aggregation(_aggregation),
		setting(_setting),
		taskId(_taskId)
{

}

AggregationTask::~AggregationTask()
{

}

void AggregationTask::run()
{
	Stopwatch stopwatch;
	stopwatch.start();

	cout << "[AggregationTask#" << taskId << "] Did work in: " 
		<< stopwatch.elapsed() / 1000.0f << " seconds. Current time: "  
		<< (aggregation -> taskCurrentTime(taskId)).epochMicroseconds() << endl;	

	//aggregation -> increaseTaskTime(setting.name, taskId);

	stopwatch.stop();
}
