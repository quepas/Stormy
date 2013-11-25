#include "DBAggregation.h"

#include <iostream>
#include <postgresql/soci-postgresql.h>
#include "../../common/Utils.h"

using namespace Stormy;
using namespace Data;
using namespace soci;
using namespace std;

DBAggregation::DBAggregation( Database* _dbAggregation, DBStorage* _dbStorage )
	:	dbAggregation(_dbAggregation),
		dbStorage(_dbStorage)
{
	connect();
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

bool DBAggregation::insertPeriod( AggregationSetting aggregation )
{
	TRY
	sql << "INSERT INTO aggregate_period VALUES(:name, :period)",
		use(aggregation.name), use(aggregation.interval);
	return true;
	CATCH_MSG("AggregationDB] insertAggregationPeriod(): ")
	return false;
}

bool DBAggregation::insertPeriods( vector<AggregationSetting> settings )
{
	bool result = true;
	Utils::forEach(settings, [&](AggregationSetting setting) {
		if(!insertPeriod(setting))
			result = false;
	});
	return result;
}

bool DBAggregation::createTask( AggregationSetting aggregation, Station station )
{
	return false;
}
