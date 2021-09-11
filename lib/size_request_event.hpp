#ifndef FRUIT_LIB_SIZEREQUESTEVENT_HPP
#define FRUIT_LIB_SIZEREQUESTEVENT_HPP

#include "./viewport_size.hpp"

namespace fruit
{
	struct SizeRequestResult
	{
		ViewportSize min_size;
		ViewportSize max_size;
	};

	/**
	 * \brief An event sent to ask about the size of an object
	 *
	 * \ingroup Events
	 */
	struct SizeRequestEvent
	{
		using result_type =  SizeRequestResult;
		ViewportSize domain_size;
	};
}

#endif