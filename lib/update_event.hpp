#ifndef FRUIT_UPDATE_EVENT_HPP
#define FRUIT_UPDATE_EVENT_HPP

#include "./pixel.hpp"
#include "./world_clock.hpp"

#include <cstdint>

namespace fruit
{
	struct UpdateEventSw
	{
		Pixel* buffer;
		int width;
		int height;
		uint64_t frame_number;
		WorldClock time_point;
	};
}

#endif