#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include "meteo_data.hpp"

namespace stormy {

class DataAcquireQueue
{
public:
  DataAcquireQueue();
  ~DataAcquireQueue();

  void Push(MeteoData data);
  MeteoData Pop();

private:
  std::mutex mutex_;
  std::queue<MeteoData> data_queue_;  
};

// ~~ stormy::DataAcquireQueue
}