#ifndef FRUIT_CHARTYPES_HPP
#define FRUIT_CHARTYPES_HPP

#include <cstdint>

namespace fruit
{
	class GlyphIndex
	{
	public:
		GlyphIndex() = default;

		explicit GlyphIndex(uint32_t value): m_value {value}{}

		uint32_t value() const
		{ return m_value; }

		bool operator==(GlyphIndex const&) const = default;
		bool operator!=(GlyphIndex const&) const = default;

	private:
		uint32_t m_value;
	};

	using CharCodepoint = char32_t;
}

#endif