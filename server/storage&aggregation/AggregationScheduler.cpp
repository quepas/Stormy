#include "AggregationScheduler.h"

#include <iostream>
#include "AggregationTask.h"

using namespace Stormy;
using namespace std;

AggregationScheduler::AggregationScheduler( DBAggregation* _dbAggregation )
	:	dbAggregation(_dbAggregation)
{

}

AggregationScheduler::~AggregationScheduler()
{

}

void AggregationScheduler::scheduleAggregation( AggregationSetting setting )
{
	DBStorage* storage = dbAggregation -> getStorageDatabase();	
	auto stationUIds = storage -> GetStationUIDs();

	for(uint32 i = 0; i < stationUIds.size(); ++i) {		
		uint32 id = dbAggregation -> createOrRefreshTask(stationUIds[i], setting);		
		if(id > 0) {
			cout << "[AggregationScheduler] Create task \"" << setting.name
				<< "\" for station: " 
				<< storage -> GetStationName(stationUIds[i]) << endl;
			
			schedule(new AggregationTask(dbAggregation, setting, id), 
				0, setting.taskRefresh * 1000);
		}		
	}	
}

void AggregationScheduler::scheduleManyAggregations( 
	vector<AggregationSetting> const& settings )
{
	Utils::forEach(settings, [&](AggregationSetting setting) {
		scheduleAggregation(setting);
	});
}
