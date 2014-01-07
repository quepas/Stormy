#pragma once

#include "db_aggregate.h"
#include "../../common/db_setting.h"

#include <string>
#include <vector>
#include <Poco/Logger.h>

namespace stormy {
  namespace analysis {

class OperationMgr
{
public:
  OperationMgr(
    std::string base_dir, 
    common::db::Setting db_aggregate_setting);
  ~OperationMgr();

  int Reload();
  std::vector<std::string> GetOperations() {
    return operations_;
  }  
  std::string GetBaseDir() {
    return base_dir_;
  }
private:
  int LoadFromDirectory();
  void InsertIntoDatabase();

  std::string base_dir_;
  std::vector<std::string> operations_;
  db::Aggregate aggregate_database_;
  Poco::Logger& logger_;
};
// ~~ stormy::analysis::OperationMgr
}}
