#pragma once

#include <cstdint>
#include <Poco/Logger.h>
#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace rest {
   
class Service : public Poco::Util::ServerApplication
{
public:
  Service(uint16_t port = 8070);
  ~Service();

protected:
  void initialize(Poco::Util::Application& self) override;
  void uninitialize() override;
  int main(const std::vector<std::string>& args) override;

private:
  uint16_t port_;
  Poco::Logger& logger_;
};
// ~~ stormy::rest::Service
}}