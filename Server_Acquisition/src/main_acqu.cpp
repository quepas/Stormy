#ifdef STORMY_MAIN

#include <Poco/Exception.h>
#include <Poco/StreamCopier.h>
#include <Poco/Timespan.h>
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Connection.h"
#include "Poco/MongoDB/Database.h"
#include "Poco/MongoDB/Cursor.h"
#include "Poco/MongoDB/Array.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <yaml-cpp/yaml.h>

#include <memory>
#include <iostream>
#include <string>
#include <vector>

struct StationCfg
{
  std::string name;
  std::string url;
  unsigned int update_time;
  std::string read_script;
};

int main() {
  std::cout << "Welcome to StormyMain" << std::endl;

  std::vector<StationCfg> station_cfgs;
  try {
    YAML::Node root = YAML::LoadFile("config/meteo_stations.yaml");
    unsigned int i = 0;
    for (auto& node : root)
    {
      try {
        StationCfg cfg = {
          node["name"].as<std::string>(),
          node["url"].as<std::string>(),
          node["update_time"].as<unsigned>(),
          node["read_script"].as<std::string>()};
        station_cfgs.push_back(cfg);
        ++i;
      }
      catch (const YAML::ParserException& ex) {
        std::cout << "Parser exception: " << ex.what() << std::endl;
      }
    }
    std::cout << "Successfully loaded " << i
      << " station configurations." << std::endl;
  }
  catch (const YAML::BadFile& ex) {
    std::cout << "Bad file exception: " << ex.what() << std::endl;
  }

  Poco::Net::HTTPStreamFactory::registerFactory();

  for (auto& cfg : station_cfgs) {
    try {
      Poco::URI uri(cfg.url);
      std::unique_ptr<std::istream> stream(Poco::URIStreamOpener::defaultOpener().open(uri));
      //Poco::StreamCopier::copyStream(*stream.get(), std::cout);
      std::cout << "Completed for: " << cfg.name << std::endl;
    }
    catch (const Poco::Exception& ex) {
      std::cout << ex.displayText() << std::endl;
    }
  }

  Poco::MongoDB::Connection connection("localhost", 27017);
  Poco::MongoDB::Database db("test2");
  Poco::SharedPtr<Poco::MongoDB::InsertRequest> insert_stations_cfg = db.createInsertRequest("stations");

  for (auto& cfg : station_cfgs) {
    insert_stations_cfg->addNewDocument()
      .add("name", cfg.name)
      .add("read_script", cfg.read_script)
      .add("update_time",(int) cfg.update_time)
      .add("url", cfg.url);
  }
  std::cout << "Inserting " << insert_stations_cfg->documents().size() << " stations cfg." << std::endl;
  connection.sendRequest(*insert_stations_cfg);
  std::string lastError = db.getLastError(connection);
  if (!lastError.empty())
  {
    std::cout << "Last Error: " << db.getLastError(connection) << std::endl;
  }
  getchar();
  return 0;
}

#endif