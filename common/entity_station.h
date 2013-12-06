#pragma once

#include <cstdint>
#include <string>

namespace stormy {
	namespace common {
		namespace entity {

struct Station
{
	std::string uid;
	std::string name;
	std::string url;
	uint32_t refresh_time;
	std::tm last_update;	
};
// ~~ stormy::common::entity::Station
}}}