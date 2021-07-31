#ifndef FRUIT_LOCATIONEVENT_HPP
#define FRUIT_LOCATIONEVENT_HPP

#include "./button_state.hpp"
#include "./point.hpp"

namespace fruit
{
	struct LocationEvent
	{
		Point<float> loc;
		int button;
		ButtonState state;
	};
}

#endif