#pragma once

#include <iostream>
#include <thread>
#include <string>
#include <exception>
#include "py_read_script.hpp"

#include <boost/network.hpp>
#include <boost/timer.hpp>

using namespace boost::network;
using namespace boost::network::http;

using std::chrono::milliseconds;
using std::cout;
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

  DataAcquireTask(const Station& station) 
    : done(false),
      read_script(station.script_name) {
    thread_ = thread([this, station]{
      while (!done)
      {
        boost::timer timer;
        try {          
          //read_script();                
          boost::network::uri::uri uri(station.url);          
          if (uri.is_valid()) 
          {
            client::request request_(uri);            
            request_ << header("Connection", "close");
            client client_;
            client::response response_ = client_.get(request_);            
            std::string s = body(response_);
            std::cout << ".";
          } 
          else
          {
            std::cout << "\nFailed: " << station.url;
          }          
        }
        catch (const std::exception& ex) {
          std::cout << "ex: " << ex.what() << std::endl;          
        }
        double time_elapsed = timer.elapsed()*1000;
        double recompensate_time = std::floor(station.refresh_time * 1000 - time_elapsed);
        std::cout << "\nrecomp: " << recompensate_time << ", elapsed: " << time_elapsed << ", refresh: " << station.refresh_time * 1000 <<std::endl;
        // time recompensing
        if (recompensate_time > 0)
          sleep_for(milliseconds(static_cast<int>(recompensate_time)));
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
  PyReadScript read_script;
  mutable bool done;  
};
// ~~ stormy::DataAcquireTask
}