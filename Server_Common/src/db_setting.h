#pragma once

#include <string>
#include <Poco/NumberFormatter.h>

using std::string;
using Poco::NumberFormatter;

namespace stormy {
  namespace common {
    namespace db {

struct Setting
{
  string name;
  string host;
  unsigned int port;
  string database;
  string user;
  string password;
  static const unsigned int defaultPort = 5432;

  string ToString() {
    string result;
    result.append("[name: " + name + ", ");
    result.append("host: " + host + ", ");
    result.append("port: " + NumberFormatter::format(port) + ", ");
    result.append("database: " + database + ", ");
    result.append("user: " + user + ", ");
    result.append("password: " + password + "]");
    return result;
  }

  string AsConnectionString() {
    string result;
    result.append("dbname=" + database);
    result.append(" host=" + host);
    result.append(" port=" + NumberFormatter::format(port));
    result.append(" user=" + user);
    result.append(" password=" + password);
    return result;
  }
};
// ~~ stormy::common::db::Setting
}}}