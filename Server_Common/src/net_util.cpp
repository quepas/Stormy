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
using std::unique_ptr;

namespace stormy {
  namespace net {

string FetchWebsite(string website_url)
{
  string content;
  try {
    URI uri(website_url);
    HTTPStreamFactory stream_factory;
    unique_ptr<std::istream> stream(stream_factory.open(uri));
    StreamCopier::copyToString(*stream, content);
  }
  catch (const Exception& exception) {
    Logger::get("net/util").error(exception.displayText());
  }
  return content;
}

}}
// ~~ stormy::net::
