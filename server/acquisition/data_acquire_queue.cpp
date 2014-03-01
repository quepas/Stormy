#include "data_acquire_queue.hpp"

using std::lock_guard;
using std::mutex;
using std::string;

namespace stormy {

DataAcquireQueue::DataAcquireQueue()
{
}

DataAcquireQueue::~DataAcquireQueue()
{
}

void DataAcquireQueue::Push(string data)
{
  lock_guard<mutex> guard(mutex_);
  data_queue_.push(data);
}

string DataAcquireQueue::Pop()
{
  lock_guard<mutex> guard(mutex_);
  auto result = data_queue_.back();
  data_queue_.pop();
  return result;
}
// ~~ stormy::DataAcquireQueue
}