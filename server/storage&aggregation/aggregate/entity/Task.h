#pragma once

#include <string>

namespace stormy {
	namespace aggregate {
		namespace entity {

struct Task
{
	int id;
	std::string period_name;
	std::string station_uid;	
	std::tm current_ts;
};

// ~~ stormy::aggregation::entity::Task
}}}