#include "DBStorage.h"

#include <exception>
#include <iostream>

#include <soci.h>
#include <postgresql/soci-postgresql.h>

using namespace Stormy;
using namespace std;
using namespace soci;

DBStorage::DBStorage( StorageDatabase* storageDB )
	:	configuration(storageDB)
{	

}

DBStorage::~DBStorage()
{

}

void DBStorage::connect()
{
	try {
		session sql(postgresql, configuration -> asConnectionString());	
	} catch(exception const& ex) {
		cout << "[StorageDB] Connect exception: " << ex.what() << endl;
	}
}
