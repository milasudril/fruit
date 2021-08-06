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

		FontFaceHandle createFrom(std::span<std::byte const> src_buffer);

		static void free(FontFaceHandle handle)
		{
			FT_Done_Face(handle);
		}

		static void size(FontFaceHandle handle, int val)
		{
			FT_Set_Char_Size(handle, 0, 64 * val, 0, 0);
		}

		static ImageView<uint8_t> bitmap(FontFaceHandle handle, uint32_t char_index)
		{
			FT_Load_Char(handle, char_index, FT_LOAD_RENDER);
			return ImageView{handle->glyph->bitmap.buffer,
				static_cast<int>(handle->glyph->bitmap.width),
				static_cast<int>(handle->glyph->bitmap.rows)};
		}

	private:
		FT_Library m_handle;
	};
}


#endif
