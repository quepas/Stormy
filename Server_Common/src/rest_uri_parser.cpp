#include "common/rest_uri_parser.h"

#include <boost/algorithm/string.hpp>

using std::string;
using std::vector;
using std::map;
using std::make_pair;
using Poco::URI;
using boost::split;
using boost::is_any_of;
using boost::trim;

namespace stormy {
  namespace common {
    namespace rest {

URIParser::URIParser(string raw_uri)
{
  trim(raw_uri);
  uri_ = URI(raw_uri);
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
  if (!segments.empty()) {
    parameters_with_values.reserve(2);
    for (auto it = segments.begin(); it != segments.end(); ++it) {
      parameters_with_values.clear();
      split(parameters_with_values, *it, is_any_of("="));

      if (parameters_with_values.size() == 2) {
        string parameter = parameters_with_values[0];
        if (!parameter.empty()) {
          query_segments.insert(
            make_pair(parameter, parameters_with_values[1]));
        }
      }
    }
  }
  return query_segments;
}

}}}
// ~~ stormy::common::rest::URIParser
