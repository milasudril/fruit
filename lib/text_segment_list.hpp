#ifndef FRUIT_LIB_TEXTSEGMENTLIST_HPP
#define FRUIT_LIB_TEXTSEGMENTLIST_HPP

#include "./text_shape_result.hpp"
#include "./text_properties.hpp"
#include "./error_message.hpp"
#include "./text_shaper.hpp"
#include "./viewport_size.hpp"
#include "./image.hpp"

#include <string_view>
#include <memory>
#include <stdexcept>
#include <vector>

namespace fruit
{
	class TextSegmentList
	{
	public:
		TextSegmentList& text(std::vector<std::basic_string<char8_t>>&& buffer) &
		{
			m_text = std::move(buffer);
			return *this;
		}

		TextSegmentList&& text(std::vector<std::basic_string<char8_t>>&& buffer) &&
		{
			m_text = std::move(buffer);
			return std::move(*this);
		}

		TextSegmentList& direction(TextDirection val) &
		{
			m_properties.direction = val;
			return *this;
		}

		TextSegmentList&& direction(TextDirection val) &&
		{
			m_properties.direction = val;
			return std::move(*this);
		}


		TextDirection direction() const
		{
			return m_properties.direction;
		}
		TextSegmentList& language(LanguageTag const& lang) &
		{
			m_properties.language = lang;
			return *this;
		}

		TextSegmentList&& language(LanguageTag const& lang) &&
		{
			m_properties.language = lang;
			return std::move(*this);
		}

		LanguageTag const& language() const
		{
			return m_properties.language;
		}

		TextSegmentList& script(WritingSystem val) &
		{
			m_properties.script = val;
			return *this;
		}

		TextSegmentList&& script(WritingSystem val) &&
		{
			m_properties.script = val;
			return std::move(*this);
		}

		WritingSystem script() const
		{
			return m_properties.script;
		}

		std::vector<TextShapeResult> shape(TextShaper const& shaper) const
		{
			std::vector<TextShapeResult> ret;
			ret.reserve(std::size(m_text));
			std::ranges::transform(m_text, std::back_inserter(ret), [&props = m_properties, &shaper](auto const& val) {
				return fruit::shape(shaper, val, props);
			});
			return ret;
		}

	private:
		TextProperties m_properties;
		std::vector<std::basic_string<char8_t>> m_text;
	};
}


#endif
