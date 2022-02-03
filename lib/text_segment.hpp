//@	{
//@	 "dependencies_extra":[{"ref":"./text_segment.o", "rel":"implementation"}]
//@	}

#ifndef FRUIT_LIB_TEXTSEGMENT_HPP
#define FRUIT_LIB_TEXTSEGMENT_HPP

#include "./text_shape_result.hpp"
#include "./text_properties.hpp"
#include "./error_message.hpp"
#include "./text_shaper.hpp"
#include "./viewport_size.hpp"
#include "./image.hpp"

#include <string_view>
#include <memory>
#include <stdexcept>

namespace fruit
{
	class TextSegment
	{
	public:
		TextSegment& text(std::basic_string_view<char8_t> buffer) &
		{
			m_text = buffer;
			return *this;
		}

		TextSegment&& text(std::basic_string_view<char8_t> buffer) &&
		{
			m_text = buffer;
			return std::move(*this);
		}

		TextSegment& direction(TextDirection val) &
		{
			m_properties.direction = val;
			return *this;
		}

		TextSegment&& direction(TextDirection val) &&
		{
			m_properties.direction = val;
			return std::move(*this);
		}


		TextDirection direction() const
		{
			return m_properties.direction;
		}
		TextSegment& language(LanguageTag const& lang) &
		{
			m_properties.language = lang;
			return *this;
		}

		TextSegment&& language(LanguageTag const& lang) &&
		{
			m_properties.language = lang;
			return std::move(*this);
		}

		LanguageTag const& language() const
		{
			return m_properties.language;
		}

		TextSegment& script(WritingSystem val) &
		{
			m_properties.script = val;
			return *this;
		}

		TextSegment&& script(WritingSystem val) &&
		{
			m_properties.script = val;
			return std::move(*this);
		}

		WritingSystem script() const
		{
			return m_properties.script;
		}

		TextShapeResult shape(TextShaper const& shaper) const &
		{
			FRUIT_ASSERT(shaper.valid());
			return shape_impl(shaper);
		}

	private:
		TextShapeResult shape_impl(TextShaper const& shaper) const;
		TextProperties m_properties;
		std::basic_string<char8_t> m_text;
	};
}


#endif
