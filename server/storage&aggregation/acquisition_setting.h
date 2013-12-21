#pragma once

#include <cstdint>
#include <string>
#include <Poco/NumberFormatter.h>

namespace stormy {
  namespace acquisition {   

struct Setting
{
	std::string id;
	std::string name;
	std::string host;
	uint16_t port;
	uint32_t interval;	// seconds

  std::string ToString() {
		std::string result;
		result.append("[id: " + id + ", ");
		result.append("name: " + name + ", ");
		result.append("host: " + host + ", ");
		result.append("port: " + 
       Poco::NumberFormatter::format(port) + ", ");
		result.append("acqInterval: " + 
       Poco::NumberFormatter::format(interval) + "]");
		return result;
	}
};
// ~~ stormy::acquisition::Setting
}}
