#ifdef STORMY_LITE
#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <exception>
#include "data_acquire_queue.hpp"

#include <boost/network.hpp>
#include <boost/timer.hpp>

namespace net = boost::network;

using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::floor;
using std::string;
using std::thread;
using std::this_thread::sleep_for;

namespace stormy {

class DataAcquireTask
{
public:
  struct Station
  {
    std::string name;
    std::string url;
    unsigned refresh_time;
    std::string script_name;
  };

  DataAcquireTask(const Station& station, DataAcquireQueue& queue)
    : done(false),
      queue_(queue) {
    thread_ = thread([this, station]{
      while (!done)
      {
        boost::timer timer;
        try {
          net::uri::uri uri(station.url);
          if (uri.is_valid()) 
          {
            net::http::client::request request_(uri);
            request_ << net::header("Connection", "close");
            net::http::client client_;
            net::http::client::response response_ = client_.get(request_);
            string content = body(response_);
            string::size_type left_index = content.find_first_of("<");
            content = content.substr(left_index);

            queue_.Push(content);
            std::cout << ".";
          }
          else
          {
            cout << "\nFailed: " << station.url << endl;
          }
        }
        catch (const std::exception& ex) {
          cout << "ex: " << ex.what() << endl;
        }
        double time_elapsed_ms = timer.elapsed() * 1000;
        double time_recompensation_ms = floor(station.refresh_time * 1000 - time_elapsed_ms);
        if (time_recompensation_ms > 0)
          sleep_for(milliseconds(static_cast<int>(time_recompensation_ms)));
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
  DataAcquireQueue& queue_;
  mutable bool done;  
};
// ~~ stormy::DataAcquireTask
}
#endif