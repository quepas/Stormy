#include "AggregationScheduler.h"

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
	// add aggregation settings

	// scheduler aggregation
}

void AggregationScheduler::scheduleManyAggregations( vector<AggregationSetting> const& settings )
{

}
