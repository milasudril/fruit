//@	{
//@	"dependencies_extra":[{"ref":"./freetype_fontface_loader.o", "rel":"implementation"}]
//@	}

#ifndef FRUIT_FREETYPE_FONTFACE_LOADER_HPP
#define FRUIT_FREETYPE_FONTFACE_LOADER_HPP

#include "simple_types/cstring.hpp"
#include "containers/short_string.hpp"
#include "containers/long_string.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstddef>

namespace Epoch::TextRenderer
{
	class FreetypeFontfaceLoader
	{
	public:
		using FontFaceHandle = FT_Face;
		static constexpr FontFaceHandle NullHandle = nullptr;

		FreetypeFontfaceLoader();
		~FreetypeFontfaceLoader();

		FontFaceHandle createFrom(std::byte const* src_buffer, size_t src_buffer_size);

		static void free(FontFaceHandle handle)
		{
			FT_Done_Face(handle);
		}

		static void size(FontFaceHandle handle, int val)
		{
			FT_Set_Char_Size(handle, 0, 64 * val, 0, 0);
		}

		static Span2d<uint8_t> bitmap(FontFaceHandle handle, uint32_t char_index)
		{
			FT_Load_Char(handle, char_index, FT_LOAD_RENDER);
			return Span2d{handle->glyph->bitmap->buffer, handle->glyph->bitmap->rows, handle->glyph->bitmap->width};
		}

	private:
		FT_Library m_handle;
	};
}


#endif
