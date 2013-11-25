#pragma once

#include <string>
#include "../../common/Utils.h"

namespace Stormy
{
	struct Database
	{
		std::string name;
		std::string host;
		unsigned int port;
		std::string database;
		std::string user;
		std::string password;
		static const unsigned int defaultPort = 5432;

		std::string toString() {
			std::string result;
			result.append("[name: " + name + ", ");
			result.append("host: " + host + ", ");
			result.append("port: " + lexical_cast<std::string>(port) + ", ");
			result.append("database: " + database + ", ");
			result.append("user: " + user + ", ");
			result.append("password: " + password + "]");
			return result;
		}

		std::string asConnectionString() {
			std::string result;			
			result.append("dbname=" + database);
			result.append(" host=" + host);
			result.append(" port=" + lexical_cast<std::string>(port));			
			result.append(" user=" + user);
			result.append(" password=" + password);
			return result;
		}
	};	
}