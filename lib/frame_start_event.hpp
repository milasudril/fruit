#ifndef FRUIT_FRAME_START_EVENT_HPP
#define FRUIT_FRAME_START_EVENT_HPP

#include "./world_clock.hpp"

#include <cstdint>

namespace fruit
{
	struct FrameStartEvent
	{
		uint64_t frame_number;
		WorldClock time_point;
	};
}

#endif