#pragma once

#include "db_storage.h"
#include "db_aggregate.h"

#include <cstdint>
#include <Poco/Logger.h>
#include <Poco/Util/ServerApplication.h>

namespace stormy {
  namespace rest {
   
class Service : public Poco::Util::ServerApplication
{
public:
  Service(
    db::Storage* db_storage, 
    db::Aggregate* db_aggregation, 
    uint16_t port = 8070);
  ~Service();

protected:
  void initialize(Poco::Util::Application& self) override;
  void uninitialize() override;
  int main(const std::vector<std::string>& args) override;

private:
  uint16_t port_;
  Poco::Logger& logger_;

  db::Storage* db_storage_;
  db::Aggregate* db_aggregation_;
};
// ~~ stormy::rest::Service
}}