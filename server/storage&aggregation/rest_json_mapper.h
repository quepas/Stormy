#pragma once

#include <string>

namespace stormy {
  namespace rest {
    namespace json {

class Mapper 
{
public:
  static std::string prepareServerInfo();
  static std::string prepareBadResponse(std::string uri);

private:
  Mapper() {};
  Mapper(const Mapper&) {}
  Mapper& operator=(const Mapper&) {}  

  static std::string wrapAsJSONString(std::string text);
};
// ~~ stormy::rest::json::Mapper
}}}