#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <string>

namespace stormy {

class DataAcquireQueue
{
public:
  DataAcquireQueue();
  ~DataAcquireQueue();

  unsigned Size() const { return data_queue_.size(); }
  void Push(std::string data);
  std::string Pop();

private:
  std::mutex mutex_;
  std::queue<std::string> data_queue_;  
};

// ~~ stormy::DataAcquireQueue
}