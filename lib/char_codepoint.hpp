#ifndef FRUIT_CHAR_CODEPOINT_HPP
#define FRUIT_CHAR_CODEPOINT_HPP

#include <cstdint>

namespace fruit
{
	class CharCodepoint
	{
	public:
		constexpr explicit CharCodepoint(uint32_t value):m_value{value}{}

		constexpr uint32_t value() const
		{
			return m_value;
		}

	private:
		uint32_t m_value;
	};

	constexpr bool operator==(CharCodepoint a, CharCodepoint b)
	{ return a.value() == b.value(); }

	constexpr bool operator!=(CharCodepoint a, CharCodepoint b)
	{ return !(a == b); }
}

#endif