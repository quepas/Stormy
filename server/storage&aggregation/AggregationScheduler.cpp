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
	// now only for mean()
	auto stationIds = storage -> getStationIds();

	for(uint32 i = 0; i < stationIds.size(); ++i) {
		cout << "StationID :" << stationIds[i] << endl;
		uint32 id = dbAggregation -> createTask(stationIds[i], setting);
		cout << "StationID :" << id << endl;
		if(id > 0) {
			cout << "[AggregationScheduler] Create task " << setting.name
				<< " for station ID: " << stationIds[i] << endl;

			if(id != 0) {
				// scheduler aggregation
				schedule(new AggregationTask(dbAggregation, setting, id), 
					0, setting.taskRefresh * 1000);
			}
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
