#ifndef FRUIT_UPDATE_EVENT_HPP
#define FRUIT_UPDATE_EVENT_HPP

#include "./pixel.hpp"

#include <cstdint>
#include <string>

namespace fruit
{
	struct UpdateEventSw
	{
		Pixel* buffer;
		int width;
		int height;

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
		ret += ")";
		return ret;
	}
}

#endif