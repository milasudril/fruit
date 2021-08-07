//@	{
//	"dependencies_extra":[{"ref":"text_shaper.o", "rel":"implementation"}]
//@	}

#ifndef FRUIT_TEXTSHAPER_HPP
#define FRUIT_TEXTSHAPER_HPP

#include "./freetype_fontface_loader.hpp"
#include "./text_segment.hpp"
#include "./point.hpp"

#include <hb.h>
#include <hb-ft.h>

#include <memory>
#include <stdexcept>

namespace fruit
{
	namespace text_shaper_detail
	{
		struct Deleter
		{
			void operator()(hb_font_t* font)
			{
				if(font != nullptr)
				{
					hb_font_destroy(font);
				}
			}
		};

		class InitError:public std::exception
		{
		public:
			char const* what() const noexcept override
			{return "Failed to create a text shaper from the selected font";}
		};
	}

	class TextShaper
	{
	public:
		explicit TextShaper(std::reference_wrapper<FreetypeFontFace const> face):
		m_handle{hb_ft_font_create(face.get().native_handle(), nullptr)}
		{
			if(m_handle == nullptr)
			{throw text_shaper_detail::InitError{};}
		}

		bool valid() const
		{
			return m_handle != nullptr;
		}

		template<class GlyphRenderer>
		vec4_t<float> shape(TextSegment const& buffer, GlyphRenderer&& render) const
		{
			FRUIT_ASSERT(valid());
			FRUIT_ASSERT(buffer.valid());

			auto const buff = buffer.native_handle();
			auto const handle = m_handle.get();
			hb_shape(handle, buff, nullptr, 0);

			unsigned int glyph_count;
			auto glyph_info = hb_buffer_get_glyph_infos(buff, &glyph_count);
			auto glyph_pos = hb_buffer_get_glyph_positions(buff, &glyph_count);

			auto cursor_pos = Origin<float>;
			for(unsigned int k = 0; k < glyph_count; ++k)
			{
				Vector<float> const cursor_pos_delta{static_cast<float>(glyph_pos[k].x_advance),
				                                     static_cast<float>(glyph_pos[k].y_advance),
				                                     0.0f};

				Vector<float> const glyph_offset{static_cast<float>(glyph_pos[k].x_offset),
				                                 static_cast<float>(glyph_pos[k].y_offset),
				                                 0.0f};

				render(glyph_info[k].codepoint, cursor_pos + glyph_offset);
				cursor_pos += cursor_pos_delta;
			}

			return cursor_pos;
		}

	private:
		std::unique_ptr<hb_font_t, text_shaper_detail::Deleter> m_handle;
	};
}


#endif
