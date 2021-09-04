#ifndef FRUIT_TEXTELEMENT_HPP
#define FRUIT_TEXTELEMENT_HPP

#include "./text_segment.hpp"
#include "./font_face.hpp"
#include "./size_request_event.hpp"

#include <functional>

namespace fruit
{
	class TextLine
	{
	public:
		TextLine(std::reference_wrapper<FontFace const> font):
			m_text{},
			m_size{16},
			m_font{font}
		{}

		TextLine& text(std::basic_string_view<char8_t> buffer) &
		{
			m_text.text(buffer);
			return *this;
		}

		TextLine&& text(std::basic_string_view<char8_t> buffer) &&
		{
			m_text.text(buffer);
			return std::move(*this);
		}

		TextLine& direction(TextDirection val) &
		{
			m_text.direction(val);
			return *this;
		}

		TextLine&& direction(TextDirection val) &&
		{
			m_text.direction(val);
			return std::move(*this);
		}

		TextLine& language(LanguageTag const& lang) &
		{
			m_text.language(lang);
			return *this;
		}

		TextLine&& language(LanguageTag const& lang) &&
		{
			m_text.language(lang);
			return std::move(*this);
		}

		TextLine& script(WritingSystem val) &
		{
			m_text.script(val);
			return *this;
		}

		TextLine&& script(WritingSystem val) &&
		{
			m_text.script(val);
			return std::move(*this);
		}

		TextLine& size(int size) &
		{
			m_size = size;
			return *this;
		}

		TextLine&& size(int size) &&
		{
			m_size = size;
			return std::move(*this);
		}

		TextLine& font(FontFace& font) &
		{
			m_font = font;
			return *this;
		}

		TextLine&& font(FontFace& font) &&
		{
			m_font = font;
			return std::move(*this);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const
		{
			SizeRequestResult res{};

			return res;
		}

	private:
		TextSegment m_text;
		int m_size;
		std::reference_wrapper<FontFace const> m_font;
	};
}

#endif