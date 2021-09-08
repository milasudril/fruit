#ifndef FRUIT_HIT_TEST_EVENT_HPP
#define FRUIT_HIT_TEST_EVENT_HPP

#include "./point.hpp"

namespace fruit
{
	struct HitTestEvent
	{
		using result_type = bool;
		Point<int> location;
	};
}

#endif