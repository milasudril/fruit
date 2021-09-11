#ifndef FRUIT_LIB_BOUNDARYCROSSEDEVENT_HPP
#define FRUIT_LIB_BOUNDARYCROSSEDEVENT_HPP

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