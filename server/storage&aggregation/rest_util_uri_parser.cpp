#include "rest_util_uri_parser.h"

#include <boost/algorithm/string.hpp>

using std::string;
using std::vector;
using std::map;
using std::make_pair;
using Poco::URI;
using boost::split;
using boost::is_any_of;

namespace stormy {
  namespace rest {
    namespace util {

URIParser::URIParser(string raw_uri)
  : uri_(raw_uri)
{

}

URIParser::~URIParser()
{

}

vector<string> URIParser::getPathSegments()
{
  auto segments = vector<string>();
  uri_.getPathSegments(segments);
  return segments;
}

map<string, string> URIParser::getQuerySegments()
{  
  auto segments = vector<string>();
  auto parameters_with_values = vector<string>();
  auto query_segments = map<string, string>();

  split(segments, uri_.getQuery(), is_any_of("&"));
  if(!segments.empty()) {
    parameters_with_values.reserve(2);
    for (auto it = segments.begin(); it != segments.end(); ++it) {
      parameters_with_values.clear();
      split(parameters_with_values, *it, is_any_of("="));

      if(parameters_with_values.size() == 2) {
        query_segments.insert(
          make_pair(parameters_with_values[0], parameters_with_values[1]));
      }
    }
  }
  return query_segments;
}
// ~~ stormy::rest::util::URIParser
}}}
