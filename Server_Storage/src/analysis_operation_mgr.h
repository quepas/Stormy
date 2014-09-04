#pragma once

#include "db_aggregate.h"
#include "settings.hpp"

#include <string>
#include <vector>
#include <Poco/Logger.h>

namespace stormy {
  namespace analysis {

class OperationMgr
{
public:
  OperationMgr(std::string base_dir, DatabaseSetting setting);
  ~OperationMgr();

  int Reload();
  const std::vector<std::string>& operations() { return operations_; }
  const std::string& base_dir() { return base_dir_; }
private:
  int LoadFromDirectory();
  void InsertIntoDatabase();

  std::string base_dir_;
  std::vector<std::string> operations_;
  db::Aggregate aggregate_database_;
  Poco::Logger& logger_;
};

}}
// ~~ stormy::analysis::OperationMgr
