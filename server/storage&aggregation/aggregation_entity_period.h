#pragma once

#include <ctime>
#include <string>

namespace stormy {
	namespace aggregation {
		namespace entity {

struct Period
{
	std::string name;
	std::time_t interval;
};
// ~~ stormy::aggregation::entity::Period
}}}