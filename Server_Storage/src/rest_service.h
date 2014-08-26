#pragma once

#include "settings.hpp"

#include <cstdint>
#include <Poco/Logger.h>
#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace rest {
   
class Service : public Poco::Util::ServerApplication
{
public:
  Service(
    DatabaseSetting db_storage, 
    DatabaseSetting db_aggregation,
    unsigned port = 8070);
  ~Service();

protected:
  void initialize(Poco::Util::Application& self) override;
  void uninitialize() override;
  int main(const std::vector<std::string>& args) override;

private:
  unsigned port_;
  Poco::Logger& logger_;

  DatabaseSetting db_storage_;
  DatabaseSetting db_aggregation_;
};

}}
// ~~ stormy::rest::Service
