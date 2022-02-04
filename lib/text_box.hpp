//@	{"dependencies_extra":[{"ref":"./text_box.o", "rel":"implementation"}]}

#ifndef FRUIT_LIB_TEXTBOX_HPP
#define FRUIT_LIB_TEXTBOX_HPP

#include "./text_segment_list.hpp"
#include "./font_face.hpp"
#include "./size_request_event.hpp"
#include "./pixel.hpp"

#include <functional>

namespace fruit
{
	class TextBox
	{
	public:
		TextBox(std::reference_wrapper<FontFace const> font):
			m_text{},
			m_char_height{16},
			m_font{font},
			m_size{}
		{}

		TextBox& text(std::vector<std::basic_string<char8_t>>&& buffer) &
		{
			m_text.text(std::move(buffer));
			m_render_result = {};
			return *this;
		}

		TextBox&& text(std::vector<std::basic_string<char8_t>>&& buffer) &&
		{
			m_text.text(std::move(buffer));
			m_render_result = {};
			return std::move(*this);
		}

		TextBox& direction(TextDirection val) &
		{
			m_text.direction(val);
			m_render_result = {};
			return *this;
		}

		TextBox&& direction(TextDirection val) &&
		{
			m_text.direction(val);
			m_render_result = {};
			return std::move(*this);
		}

		TextBox& language(LanguageTag const& lang) &
		{
			m_text.language(lang);
			m_render_result = {};
			return *this;
		}

		TextBox&& language(LanguageTag const& lang) &&
		{
			m_text.language(lang);
			m_render_result = {};
			return std::move(*this);
		}

		TextBox& script(WritingSystem val) &
		{
			m_text.script(val);
			m_render_result = {};
			return *this;
		}

		TextBox&& script(WritingSystem val) &&
		{
			m_text.script(val);
			m_render_result = {};
			return std::move(*this);
		}

		TextBox& char_height(int size) &
		{
			m_char_height = std::max(size, 8);
			m_render_result = {};
			return *this;
		}

		TextBox&& char_height(int size) &&
		{
			m_char_height = std::max(size, 16);
			m_render_result = {};
			return std::move(*this);
		}

		TextBox& font(FontFace& font) &
		{
			m_font = font;
			m_render_result = {};
			return *this;
		}

		TextBox&& font(FontFace& font) &&
		{
			m_font = font;
			m_render_result = {};
			return std::move(*this);
		}

		SizeRequestResult handle(SizeRequestEvent const&) const
		{
			if(std::size(m_render_result.first) == 0) [[unlikely]]
			{
				handle_no_result(SizeRequestEvent{});
			}
			return m_size;
		}

		void compose(image_span<Pixel> output_buffer, Point<int> origin, Pixel color) const;

	private:
		TextSegmentList m_text;
		int m_char_height;
		std::reference_wrapper<FontFace const> m_font;

		void handle_no_result(SizeRequestEvent const&) const;

		void do_render() const;

		mutable std::pair<std::vector<TextShapeResult>, std::vector<TextAlphaMask>> m_render_result;
		mutable SizeRequestResult m_size;
	};
}

#endif