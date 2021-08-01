#ifndef FRUIT_UPDATE_EVENT_HPP
#define FRUIT_UPDATE_EVENT_HPP

#include "./pixel.hpp"
#include "./world_clock.hpp"

#include <cstdint>
#include <string>

namespace fruit
{
	struct UpdateEventSw
	{
		Pixel* buffer;
		int width;
		int height;
		uint64_t frame_number;
		WorldClock::time_point time_point;

		constexpr bool operator==(UpdateEventSw const&) const = default;
		constexpr bool operator!=(UpdateEventSw const&) const = default;
	};

	std::string to_string(UpdateEventSw const& event)
	{
		std::string ret{"buffer "};
		ret += std::to_string(reinterpret_cast<intptr_t>(event.buffer));
		ret +=" (";
		ret += std::to_string(event.width);
		ret += " x ";
		ret += std::to_string(event.width);
		ret += ") ";
		ret += std::to_string(event.frame_number);
		ret += ", ";
		ret += std::to_string(event.time_point.time_since_epoch().count());
		return ret;
	}
}

#endif