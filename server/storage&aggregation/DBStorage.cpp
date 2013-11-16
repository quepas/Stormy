#include "DBStorage.h"

#include <exception>
#include <iostream>

#include <postgresql/soci-postgresql.h>

using namespace Stormy;
using namespace Stormy::Data;
using namespace std;
using namespace soci;

DBStorage::DBStorage( StorageDatabase* storageDB )
	:	configuration(storageDB)
{	
	connect();
}

DBStorage::~DBStorage()
{

}

void DBStorage::connect()
{
	TRY
	sql.open(postgresql, configuration -> asConnectionString());	
	CATCH_MSG("[StorageDB] Connect exception: ")
}

unsigned int DBStorage::countStation()
{
	unsigned int count = 0;
	TRY
	sql << "SELECT count(*) FROM station", into(count);
	CATCH
	return count;
}

void DBStorage::insertStation( Station* station )
{
	if(station) {
		TRY
		sql << "INSERT INTO station(uid, name, url, refresh)"
			"values(:uid, :name, :url, :refresh)",
			use(station -> id), use(station -> name), 
			use(station -> url), use(station -> refreshTime);
		CATCH		
	}
}
