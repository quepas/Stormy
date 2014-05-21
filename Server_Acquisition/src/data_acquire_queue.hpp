#pragma once

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
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
  boost::mutex mutex_;
  std::queue<std::string> data_queue_;  
};

// ~~ stormy::DataAcquireQueue
}
