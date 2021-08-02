#ifndef FRUIT_SIZEREQUESTEVENT_HPP
#define FRUIT_SIZEREQUESTEVENT_HPP

#include "./viewport_size.hpp"

namespace fruit
{
	struct SizeRequestEvent
	{
		using result_type =  ViewportSize;
	};
}

#endif