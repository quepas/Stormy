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
	CATCH_MSG("[StorageDB] Count station: ")
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
		CATCH_MSG("[StorageDB] Insert station: ")		
	}
}

void DBStorage::clearAllStation()
{
	TRY
	sql << "DELETE FROM station";
	CATCH_MSG("[StorageDB] Clear all station: ")
}

Station* DBStorage::getStationByUID( string uid )
{
	Station* result = new Station();
	TRY
	sql << "SELECT uid, name, url, refresh FROM station WHERE uid = :uid",
		into(result -> id), into(result -> name), into(result -> url),
		into(result -> refreshTime), use(uid);
	CATCH_MSG("[StorageDB] Get station by UID: ")
	return result;
}
