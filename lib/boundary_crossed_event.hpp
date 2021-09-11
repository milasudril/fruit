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
		static constexpr float domain_left = 1.0f;
		static constexpr float domain_entered = -1.0f;
		float direction;
	};

	constexpr bool entered_domain(BoundaryCrossedEvent const& event)
	{
		return event.direction < 0.0f;
	}

	constexpr bool left_domain(BoundaryCrossedEvent const& event)
	{
		return event.direction > 0.0f;
	}
}

#endif