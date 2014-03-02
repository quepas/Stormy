#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <atomic>

#include <yaml-cpp/yaml.h>
#include "data_acquire_task.hpp"
#include "py_read_script.hpp"

int main(int argc, char* argv [])
{
  std::cout << "### Welcome to StormyLite ###"
    << "\n* (press key to exit)"
    << "\n* (enjoy this!)"
    << "\n\nData acquire in progress";
  std::vector<stormy::DataAcquireTask*> tasks;
  stormy::DataAcquireQueue queue;

  try {
    YAML::Node root = YAML::LoadFile("../config/meteo_stations_config.yaml");
    int i = 0;
    for (auto it = root.begin(); it != root.end(); ++it)
    {
      ++i;
      try {
        stormy::DataAcquireTask::Station station = {
          (*it)["name"].as<std::string>(),
          (*it)["url"].as<std::string>(),
          (*it)["refreshTime"].as<unsigned>(),
          (*it)["parserClass"].as<std::string>()
        };
        tasks.push_back(new stormy::DataAcquireTask(station, queue));
      }
      catch (const YAML::ParserException& ex) {
        std::cout << "Parser ex: " << ex.what() << std::endl;
      }
    }
  }
  catch (const YAML::BadFile& ex) {
    std::cout << "Bad file ex: " << ex.what() << std::endl;
  }
  
  std::atomic<bool> done(false);
  std::thread read_thread([&queue, &done](){
    stormy::PyReadScript read_script("../StacjameteoReader.py");
    while (!done.load()) {
      std::cout << "Queue:" << queue.Size() << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
      if (queue.Size() > 0) {
        read_script(queue.Pop());
      }
    }
  });
  getchar();
  done.store(true);
  read_thread.join();
  for (auto& entry : tasks) {
    entry->Stop();
  }
  return EXIT_SUCCESS;
}
