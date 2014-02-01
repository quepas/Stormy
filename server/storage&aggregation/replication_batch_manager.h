#pragma once

#include "replication_batch.h"

#include <string>
#include <map>

namespace stormy {
  namespace replication {

class BatchMgr
{
public:

  void RegisterBatch(std::string batch_id);
  void PrepareBatch(std::string batch_id);
  void CancelBatch(std::string batch_id);

  template<typename T>
  void AddBatchValue(std::string batch_id, const T& value);

  bool IsBatchPrepared(std::string batch_id);
  bool IsBatchRegistered(std::string batch_id);

  BatchMgr& getInstance() 
  {
    static BatchMgr instance;
    return instance;
  }
private:
  BatchMgr();
  BatchMgr(const BatchMgr&);
  BatchMgr& operator=(const BatchMgr&);

  void RemoveBatch(std::string batch_id);

  std::map<std::string, Batch> batch_data_;
  std::map<std::string, DevoidBatch> devoid_batch_data_;
};

template<typename T>
void BatchMgr::AddBatchValue(std::string batch_id, const T& value)
{

}
// ~~ stormy::replication::BatchMgr
}}