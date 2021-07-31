#ifndef FRUIT_UPDATE_EVENT_HPP
#define FRUIT_UPDATE_EVENT_HPP

#include "./world_clock.hpp"

#include <cstdint>

namespace fruit
{
	struct UpdateEvent
	{
		uint64_t frame_number;
		WorldClock time_point;
	};
}

#endif