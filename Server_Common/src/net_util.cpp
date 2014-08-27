#include "net_util.hpp"

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/URI.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/HTTPStreamFactory.h>
#include <memory>

using Poco::Exception;
using Poco::Logger;
using Poco::URI;
using Poco::StreamCopier;
using Poco::Net::HTTPStreamFactory;
using std::string;
using std::to_string;
using std::unique_ptr;

namespace stormy {
  namespace net {

Logger& logger = Logger::get("main");

string FetchWebsite(const string& website_url)
{
  string content;
  try {
    URI uri(website_url);
    HTTPStreamFactory stream_factory;
    unique_ptr<std::istream> stream(stream_factory.open(uri));
    StreamCopier::copyToString(*stream, content);
  }
  catch (const Exception& exception) {
    logger.error(exception.displayText());
  }
  return content;
}

string PrepareUrlResource(const string& resource)
{
  if (resource.empty()) return resource;
  if (resource[0] != '/') return "/" + resource;
  return resource;
}

string PrepareHttpUrl(const string& host, const string& resource)
{
  return "http://" + host + PrepareUrlResource(resource);
}

string PrepareHttpUrl(const string& host, unsigned port, const string& resource)
{
  return "http://" + host + ":" + to_string(port) + PrepareUrlResource(resource);
}

}}
// ~~ stormy::net::
