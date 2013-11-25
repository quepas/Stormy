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

uint32 DBAggregation::createTask( AggregationSetting aggregation, Station station )
{
	return 0;
}

vector<string> DBAggregation::getPeriodNames()
{
	vector<string> result(countPeriod());
	TRY
	sql << "SELECT name FROM aggregate_period", into(result);
	CATCH_MSG("[AggregationDB] getPeriodNames(): ")
	return result;
}

uint32 DBAggregation::countPeriod()
{
	uint32 count = 0;
	TRY
	sql << "SELECT count(*) FROM aggregate_period", into(count);
	CATCH_MSG("[AggregationDB] countPeriod(): ")
	return count;
}
