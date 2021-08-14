#ifndef FRUIT_SIZEREQUESTEVENT_HPP
#define FRUIT_SIZEREQUESTEVENT_HPP

#include "./viewport_size.hpp"

namespace fruit
{
	struct SizeRequestResult
	{
		ViewportSize min_size;
		ViewportSize max_size;
	};

	struct SizeRequestEvent
	{
		using result_type =  SizeRequestResult;
	};
}

#endif