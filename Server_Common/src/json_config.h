#pragma once

#include <json/value.h>
#include <exception>
#include <string>

namespace stormy {
  
class JSONConfig
{
public:
  class no_file_exception;
  class bad_file_exception;

  JSONConfig(const std::string& file_path);
  virtual ~JSONConfig();

  const Json::Value& operator[](const std::string& key) const { return root_[key]; }
  const Json::Value& root() const { return root_; }
protected:
  Json::Value root_;
};

// exceptions
class JSONConfig::no_file_exception : public std::exception 
{
public:
  no_file_exception(const char* message) : exception(message) {}
};
class JSONConfig::bad_file_exception : public std::exception 
{
public:
  bad_file_exception(const char* message) : exception(message) {}
};
// ~~ stormy::JSONConfig
}