#ifndef FRUIT_TYPINGEVENT_HPP
#define FRUIT_TYPINGEVENT_HPP

#include "./scancode.hpp"
#include "./button_state.hpp"
#include "./chartypes.hpp"

namespace fruit
{
	/**
	 * \brief Describes a notification that the user is currently typing
	 *
	 * \ingroup Events
	 */
	struct TypingEvent
	{
		Scancode scancode;
		ButtonState state;
		CharCodepoint mapped_codepoint;
	};
}

#endif