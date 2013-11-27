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

	// check if data to run on exists
	ulong periodInSecond = aggregation -> 
		getPeriodAsSecond(setting.name).epochMicroseconds();
	Timestamp period = aggregation ->
		getPeriodAsSecond(setting.name);
	Timestamp currentTime = aggregation -> taskCurrentTime(taskId);
	Timestamp newestMeasure = aggregation -> getStorageDatabase() ->
		findNewestMeasureTimeFromStation(aggregation -> getStationIdFromTask(taskId));

	if(newestMeasure.epochMicroseconds() > 
		(currentTime.epochMicroseconds() + periodInSecond)) {
			cout << "Aggregujemy" << endl;
	} else {
		cout << "Jeszcze nie ma co agregowac" << endl;
	}

	aggregation -> increaseTaskTimeBySeconds(taskId, periodInSecond);

	cout << "[AggregationTask#" << taskId << "] Did work in: " 
		<< stopwatch.elapsed() / 1000.0f 
		<< " seconds. Current time: "
		<< (aggregation -> taskCurrentTime(taskId))
			.epochMicroseconds() << endl;	
	stopwatch.stop();
}
