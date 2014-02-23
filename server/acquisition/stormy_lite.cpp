#include <chrono>
#include <thread>
#include <iostream>
#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>
#include "data_acquire_task.h"

int main(int argc, char* argv [])
{
  std::cout << "### Welcome to StormyLite ###"
    << "\n* (press key to exit)"
    << "\n* (enjoy this shit!)"
    << "\n\nData acquire in progress";
  std::vector<stormy::DataAcquireTask*> tasks;

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
        tasks.push_back(new stormy::DataAcquireTask(station));
      }
      catch (const YAML::ParserException& ex) {
        std::cout << "Parser ex: " << ex.what() << std::endl;
      }
    }
  }
  catch (const YAML::BadFile& ex) {
    std::cout << "Bad file ex: " << ex.what() << std::endl;
  }

  getchar();
  for (auto& entry : tasks) {
    entry->Stop();
  }
  return EXIT_SUCCESS;
}
