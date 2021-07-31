#ifndef FRUIT_SCANCODE_HPP
#define FRUIT_SCANCODE_HPP

namespace fruit
{
	class Scancode
	{
	public:
		constexpr explicit Scancode(unsigned int value):m_value{value}{}

		constexpr unsigned int value() const
		{
			return m_value;
		}

	private:
		unsigned int m_value;
	};

	constexpr bool operator==(Scancode a, Scancode b)
	{ return a.value() == b.value(); }

	constexpr bool operator!=(Scancode a, Scancode b)
	{ return !(a == b); }
}

#endif