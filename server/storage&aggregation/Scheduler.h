#pragma once

#include <Poco/Util/Timer.h> 

namespace stormy
{
	namespace aggregate
	{
		class Scheduler : public Poco::Util::Timer
		{
			Scheduler();
			~Scheduler();
		};
	}
}