#pragma once

#include <iostream>
#include <thread>
#include <string>

using std::chrono::seconds;
using std::cout;
using std::string;
using std::thread;
using std::this_thread::sleep_for;

namespace stormy {

class DataAcquireTask
{
public:
  struct Station
  {
    string name;
    string url;
    unsigned refresh_time;
    string script_name;
  };

  DataAcquireTask(const Station& station) : done(false) {
    thread_ = thread([this, station]{
      while (!done)
      {
        cout << ".";
        sleep_for(seconds(station.refresh_time));
      }
    });
  }

  ~DataAcquireTask() {
    thread_.join();
  }

  void Stop() const { done = true; }

  DataAcquireTask(const DataAcquireTask&) = delete;
  DataAcquireTask& operator=(const DataAcquireTask&) = delete;
private:
  thread thread_;
  mutable bool done;
};
// ~~ stormy::DataAcquireTask
}
