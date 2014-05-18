#pragma once

#include <ctime>
#include <string>

namespace stormy {
	namespace aggregation {
		namespace entity {

struct Period
{
	std::string name;
	std::time_t seconds;
};
// ~~ stormy::aggregation::entity::Period
}}}