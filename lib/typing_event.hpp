#ifndef FRUIT_TYPINGEVENT_HPP
#define FRUIT_TYPINGEVENT_HPP

#include "./scancode.hpp"
#include "./button_state.hpp"
#include "./char_codepoint.hpp"

namespace fruit
{
	struct TypingEvent
	{
		Scancode scancode;
		ButtonState state;
		CharCodepoint mapped_codepoint;
	};
}

#endif