#pragma once

#include <cstdint>
#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace rest {

class Service : public Poco::Util::ServerApplication
{
public:
  Service(uint16_t port = 8080);
  ~Service();

protected:
  void initialized(Poco::Util::Application& self);
  void unitialized();
  int main(const std::vector<std::string>& args);

private:
  unsigned int port_;
};
// ~~ stormy::rest::Service
}}
