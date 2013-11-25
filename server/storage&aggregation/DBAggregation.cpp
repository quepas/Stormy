#include "DBAggregation.h"

#include <iostream>
#include <postgresql/soci-postgresql.h>
#include "../../common/Utils.h"

using namespace Stormy;
using namespace Stormy::Data;
using namespace soci;
using namespace std;

DBAggregation::DBAggregation( Database* _dbAggregation, DBStorage* _dbStorage )
	:	dbAggregation(_dbAggregation),
		dbStorage(_dbStorage)
{

}

DBAggregation::~DBAggregation()
{

}

void DBAggregation::connect()
{
	TRY
	sql.open(postgresql, dbAggregation -> asConnectionString());
	CATCH_MSG("[AggregationDB] connect(): ")
}

bool DBAggregation::insertAggregationPeriod( AggregationSettings aggregation )
{
	return false;
}

bool DBAggregation::prepareAggregationTimesForPeriod( string period )
{
	return false;
}

bool DBAggregation::createAggregationTask( AggregationSettings aggregation, Station station )
{
	return false;
}
