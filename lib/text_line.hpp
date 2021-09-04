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
			m_char_height{16},
			m_font{font}
		{}

		TextLine& text(std::basic_string_view<char8_t> buffer) &
		{
			m_render_result = {};
			m_text.text(buffer);
			return *this;
		}

		TextLine&& text(std::basic_string_view<char8_t> buffer) &&
		{
			m_render_result = {};
			m_text.text(buffer);
			return std::move(*this);
		}

		TextLine& direction(TextDirection val) &
		{
			m_render_result = {};
			m_text.direction(val);
			return *this;
		}

		TextLine&& direction(TextDirection val) &&
		{
			m_render_result = {};
			m_text.direction(val);
			return std::move(*this);
		}

		TextLine& language(LanguageTag const& lang) &
		{
			m_render_result = {};
			m_text.language(lang);
			return *this;
		}

		TextLine&& language(LanguageTag const& lang) &&
		{
			m_render_result = {};
			m_text.language(lang);
			return std::move(*this);
		}

		TextLine& script(WritingSystem val) &
		{
			m_render_result = {};
			m_text.script(val);
			return *this;
		}

		TextLine&& script(WritingSystem val) &&
		{
			m_render_result = {};
			m_text.script(val);
			return std::move(*this);
		}

		TextLine& char_height(int size) &
		{
			m_render_result = {};
			m_char_height = size;
			return *this;
		}

		TextLine&& char_height(int size) &&
		{
			m_render_result = {};
			m_char_height = size;
			return std::move(*this);
		}

		TextLine& font(FontFace& font) &
		{
			m_render_result = {};
			m_font = font;
			return *this;
		}

		TextLine&& font(FontFace& font) &&
		{
			m_render_result = {};
			m_font = font;
			return std::move(*this);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const
		{
			if(!m_render_result)
			{
				auto shape_res = m_text.shape(TextShaper{m_font, m_char_height});
				if(shape_res.glyph_count() == 0)
				{
					auto const min_size = is_horizontal(shape_res.direction())?
						ViewportSize{0, shape_res.char_height()} : ViewportSize{shape_res.char_height(), 0};
					return SizeRequestResult{min_size, min_size};
				}
				m_render_result = render(shape_res);
			}
			auto const min_size = ViewportSize{m_render_result->width(), m_render_result->height()};
			return SizeRequestResult{min_size, min_size};
		}

	private:
		TextSegment m_text;
		int m_char_height;
		std::reference_wrapper<FontFace const> m_font;

		mutable std::optional<Image<uint8_t>> m_render_result;
	};
}

#endif