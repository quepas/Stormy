#include "analysis_operation_mgr.h"
#include "aggregation_entity_operation.h"

#include <boost/algorithm/string/erase.hpp>
#include <boost/filesystem.hpp>

using boost::erase_first;
using boost::filesystem::path;
using boost::filesystem::is_directory;
using boost::filesystem::exists;
using boost::filesystem::directory_iterator;
using Poco::Logger;
using std::string;
using std::vector;

namespace stormy {
  namespace analysis {

OperationMgr::OperationMgr(
  string base_dir, 
  DatabaseSetting setting)
  : logger_(Logger::get("main")),
    aggregate_database_(setting),
    base_dir_(base_dir)
{
  Reload();
}

OperationMgr::~OperationMgr()
{

}

int OperationMgr::Reload()
{
  int loaded_files = LoadFromDirectory();
  InsertIntoDatabase();
  return loaded_files;
}

int OperationMgr::LoadFromDirectory()
{
  int loaded_files = 0;
  path dir_path(base_dir_);
  directory_iterator end;
  for (directory_iterator dir_it(dir_path); dir_it != end; ++dir_it) {
    if (dir_it->path().extension() == ".py") {
      string operation = dir_it->path().filename().string();
      erase_first(operation, ".py");
      operations_.push_back(operation);
      ++loaded_files;
    }
  }
  return loaded_files;
}

void OperationMgr::InsertIntoDatabase()
{
  for (auto& operation_name : operations_) {
    aggregation::entity::Operation operation;
    operation.name = operation.analysis_method = operation_name;

    if (!aggregate_database_.IfOperationExsist(operation_name)) {
      aggregate_database_.InsertOperation(operation);
    }
  }
}

}}
// ~~ stormy::analysis::OperationMgr
