#pragma once

#include <string>
#include <vector>
#include <map>
#include <Poco/URI.h>

namespace stormy {
  namespace rest {
    namespace util {

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
// ~~ stormy::rest::util::URIParser
}}}