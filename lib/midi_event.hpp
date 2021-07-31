#ifndef FRUIT_MIDIEVENT_HPP
#define FRUIT_MIDIEVENT_HPP

#include "./vector.hpp"

#include <array>

namespace fruit
{
	struct MidiEvent
	{
		std::array<uint8_t, 3> bytes;
	};
}

#endif