//@	{
//@	"dependencies_extra":[{"ref":"./freetype_fontface_loader.o", "rel":"implementation"}]
//@	,"dependencies":[{"ref":"freetype2","origin":"pkg-config"}]
//@	}

#ifndef FRUIT_FREETYPE_FONTFACE_LOADER_HPP
#define FRUIT_FREETYPE_FONTFACE_LOADER_HPP

#include "./image_view.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstddef>
#include <cstdint>

namespace fruit
{
	class FreetypeFontfaceLoader
	{
	public:
		using FontFaceHandle = FT_Face;
		static constexpr FontFaceHandle NullHandle = nullptr;

		FreetypeFontfaceLoader();
		~FreetypeFontfaceLoader();

		[[nodiscard]] FontFaceHandle createFrom(std::span<std::byte const> src_buffer);

		static void free(FontFaceHandle handle)
		{
			FT_Done_Face(handle);
		}

		static void size(FontFaceHandle handle, int val)
		{
			FT_Set_Pixel_Sizes(handle, 0, val);
		}

		static ImageView<uint8_t const> bitmap(FontFaceHandle handle, uint32_t char_index)
		{
			FT_Load_Char(handle, char_index, FT_LOAD_RENDER);
			return ImageView<uint8_t const>{handle->glyph->bitmap.buffer,
				static_cast<int>(handle->glyph->bitmap.width),
				static_cast<int>(handle->glyph->bitmap.rows)};
		}

	private:
		FT_Library m_handle;
	};
}


#endif
