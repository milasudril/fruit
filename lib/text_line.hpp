//@	{"dependencies_extra":[{"ref":"./text_line.o", "rel":"implementation"}]}

#ifndef FRUIT_LIB_TEXTLINE_HPP
#define FRUIT_LIB_TEXTLINE_HPP

#include "./text_segment.hpp"
#include "./font_face.hpp"
#include "./size_request_event.hpp"
#include "./pixel.hpp"

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
			m_text.text(buffer);
			m_render_result.reset();
			return *this;
		}

		TextLine&& text(std::basic_string_view<char8_t> buffer) &&
		{
			m_text.text(buffer);
			m_render_result.reset();
			return std::move(*this);
		}

		TextLine& direction(TextDirection val) &
		{
			m_text.direction(val);
			m_render_result.reset();
			return *this;
		}

		TextLine&& direction(TextDirection val) &&
		{
			m_text.direction(val);
			m_render_result.reset();
			return std::move(*this);
		}

		TextLine& language(LanguageTag const& lang) &
		{
			m_text.language(lang);
			m_render_result.reset();
			return *this;
		}

		TextLine&& language(LanguageTag const& lang) &&
		{
			m_text.language(lang);
			m_render_result.reset();
			return std::move(*this);
		}

		TextLine& script(WritingSystem val) &
		{
			m_text.script(val);
			m_render_result.reset();
			return *this;
		}

		TextLine&& script(WritingSystem val) &&
		{
			m_text.script(val);
			m_render_result.reset();
			return std::move(*this);
		}

		TextLine& char_height(int size) &
		{
			m_char_height = std::max(size, 16);
			m_render_result.reset();
			return *this;
		}

		TextLine&& char_height(int size) &&
		{
			m_char_height = std::max(size, 16);
			m_render_result.reset();
			return std::move(*this);
		}

		TextLine& font(FontFace& font) &
		{
			m_render_result.reset();
			m_font = font;
			return *this;
		}

		TextLine&& font(FontFace& font) &&
		{
			m_font = font;
			m_render_result.reset();
			return std::move(*this);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const
		{
			if(!m_render_result) [[unlikely]]
			{
				return handle_no_result(SizeRequestEvent{});
			}

			auto const& img = m_render_result->second;
			auto const min_size = ViewportSize{static_cast<int>(img.width()), static_cast<int>(img.height())};
			return SizeRequestResult{min_size, min_size};
		}

		void compose(image_span<Pixel> output_buffer, Point<int> origin, Pixel color) const;

	private:
		TextSegment m_text;
		int m_char_height;
		std::reference_wrapper<FontFace const> m_font;

		SizeRequestResult handle_no_result(SizeRequestEvent const&) const;

		void do_render() const;

		// FIXME: This may trigger false maybe uninitialized warnings in gcc 10
		mutable std::optional<std::pair<TextShapeResult, TextAlphaMask>> m_render_result;
	};
}

#endif