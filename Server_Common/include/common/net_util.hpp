#pragma once

#include <string>

namespace stormy {
  namespace net {

std::string FetchWebsite(const std::string& website_url);
std::string PrepareUrlResource(const std::string& resource);
std::string PrepareHttpUrl(const std::string& host, const std::string& resource);
std::string PrepareHttpUrl(const std::string& host, unsigned port, const std::string& resource);

}}
// ~~ stormy::net::
