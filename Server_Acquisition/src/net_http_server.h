#pragma once

#include "db_mongo_handler.h"

#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace net {

class HTTPServer : public Poco::Util::ServerApplication
{
public:
  HTTPServer(db::MongoHandler& db_handler, unsigned port = 8080);
  ~HTTPServer();

protected:
  void initialized(Poco::Util::Application& self);
  void unitialized();
  int main(const std::vector<std::string>& args);

private:
  db::MongoHandler& db_handler_;
  unsigned port_;
};

}}
// ~~ stormy::net::HTTPServer
