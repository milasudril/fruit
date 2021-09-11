#ifndef FRUIT_BOUNDARY_CROSSED_EVENT_HPP
#define FRUIT_BOUNDARY_CROSSED_EVENT_HPP

namespace fruit
{
	/**
	 * \brief Describes a notification that something has crossed a domain boundary
	 *
	 * \ingroup Events
	 */
	struct BoundaryCrossedEvent
	{
		float direction;
	};
}

#endif