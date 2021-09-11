#ifndef FRUIT_BALLEVENT_HPP
#define FRUIT_BALLEVENT_HPP

#include "./vector.hpp"

namespace fruit
{
	struct BallEvent
	{
		struct ScrollTag{};

		Vector<float> offset;
	};
}

#endif