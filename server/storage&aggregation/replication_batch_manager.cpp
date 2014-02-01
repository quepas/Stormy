#pragma once

#include "replication_batch_manager.h"

using std::string;
using std::make_pair;

namespace stormy {
  namespace replication {

BatchMgr::BatchMgr()
{
}

void BatchMgr::RegisterBatch(string batch_id)
{
  DevoidBatch new_dbatch;
  devoid_batch_data_.insert(make_pair(batch_id, new_dbatch));
}

void BatchMgr::PrepareBatch(string batch_id)
{
  // TODO: fill with data from db
  Batch new_batch;
  batch_data_.insert(make_pair(batch_id, new_batch));
}

void BatchMgr::CancelBatch(string batch_id)
{  
  if (!IsBatchPrepared(batch_id)) 
  {
    // TODO: cancel all batch preparation activities
  }
  RemoveBatch(batch_id);
}

bool BatchMgr::IsBatchPrepared(string batch_id)
{
  return batch_data_.find(batch_id) != batch_data_.end();
}

bool BatchMgr::IsBatchRegistered(string batch_id)
{
  return devoid_batch_data_.find(batch_id) != devoid_batch_data_.end();
}

// private:
void BatchMgr::RemoveBatch(string batch_id)
{
  if (IsBatchRegistered(batch_id))
    devoid_batch_data_.erase(batch_id);  
  if (IsBatchPrepared(batch_id))
    batch_data_.erase(batch_id);
}
// ~~ stormy::replication::BatchMgr
}}