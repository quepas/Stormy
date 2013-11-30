#pragma once

#include <string>

namespace stormy {
	namespace aggregate {
		namespace entity {

struct Period
{
	std::string name;
	std::tm interval;
};

// ~~ stormy::aggregate::entity
}}}