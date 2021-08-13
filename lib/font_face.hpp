//@	{
//@	 "dependencies_extra":[{"ref":"./font_face.o", "rel":"implementation"}]
//@	,"dependencies":[{"ref":"freetype2","origin":"pkg-config"}]
//@	}

#ifndef FRUIT_FONTFACE_HPP
#define FRUIT_FONTFACE_HPP

#include "./image_view.hpp"
#include "./vector.hpp"
#include "./chartypes.hpp"
#include "./text_direction.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include <memory>
#include <type_traits>

namespace fruit
{
	class FontfaceLoader;

	namespace detail
	{
		struct Deleter
		{
			void operator()(FT_Face face)
			{
				if(face != nullptr)
				{
					FT_Done_Face(face);
				}
			}

			void operator()(FT_Library ft)
			{
				if(ft != nullptr)
				{
					FT_Done_FreeType(ft);
				}
			}
		};
	}

	struct GlyphRenderResult
	{
		ImageView<uint8_t const> image;
		Vector<int> render_offset;
	};

	class FontFace
	{
	public:
		explicit FontFace(std::reference_wrapper<FontfaceLoader const>,
		                          std::vector<std::byte>&& src_buffer);

		FontFace& char_height(int value)
		{
			FT_Set_Pixel_Sizes(m_handle.get(), 0, value);
			m_size = value;
			return *this;
		}

		int char_height() const
		{
			return m_size;
		}

		GlyphRenderResult render(CharCodepoint char_index, TextDirection dir) const
		{
			auto handle = m_handle.get();
			FT_Load_Char(handle, char_index, FT_LOAD_RENDER);
			auto& glyph = *handle->glyph;
			return GlyphRenderResult{ImageView<uint8_t const>{glyph.bitmap.buffer,
				static_cast<int>(glyph.bitmap.width),
				static_cast<int>(glyph.bitmap.rows)},
				is_horizontal(dir) ?
					Vector{glyph.bitmap_left, char_height() - glyph.bitmap_top, 0}:
					Vector{static_cast<int>(glyph.bitmap.width/2 + glyph.bitmap_left), -glyph.bitmap_top, 0}};
		}

		GlyphRenderResult render(GlyphIndex index, TextDirection dir) const
		{
			auto handle = m_handle.get();
			FT_Load_Glyph(handle, index.value(), FT_LOAD_RENDER);
			auto& glyph = *handle->glyph;

			return GlyphRenderResult{ImageView<uint8_t const>{glyph.bitmap.buffer,
				static_cast<int>(glyph.bitmap.width),
				static_cast<int>(glyph.bitmap.rows)},
				is_horizontal(dir) ?
					Vector{glyph.bitmap_left, char_height() - glyph.bitmap_top, 0}:
					Vector{0, -glyph.bitmap_top, 0}};
		}

		FT_Face native_handle() const
		{return m_handle.get();}

	private:
		std::unique_ptr<std::remove_pointer_t<FT_Face>, detail::Deleter> m_handle;
		std::vector<std::byte> m_data;
		int m_size;
	};

	class FontfaceLoader
	{
	public:
		FontfaceLoader();

		FT_Library native_handle() const
		{ return m_handle.get(); }

	private:
		std::unique_ptr<std::remove_pointer_t<FT_Library>, detail::Deleter> m_handle;
	};
}


#endif
