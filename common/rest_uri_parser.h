#pragma once

#include <string>
#include <vector>
#include <map>
#include <Poco/URI.h>

namespace stormy {
  namespace common {
    namespace rest {    

class URIParser
{
public:
  URIParser(std::string raw_uri);
  ~URIParser();

  std::vector<std::string> getPathSegments();
  std::map<std::string, std::string> getQuerySegments();

private:
  Poco::URI uri_;
};
// ~~ stormy::common::rest::URIParser
}}}