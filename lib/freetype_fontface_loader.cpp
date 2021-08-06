//@	{
//@	"target":
//@		{
//@		"name":"freetype_fontface_loader.o","dependencies":[{"ref":"freetype2", "origin":"pkg-config"}]
//@		}
//@	}

#include "./freetype_fontface_loader.hpp"

#include "error_handling/epoch_error.hpp"

Epoch::TextRenderer::FreetypeFontfaceLoader::FreetypeFontfaceLoader()
{
	if(auto res = FT_Init_FreeType(&m_handle); res != FT_Err_Ok)
	{ throw EpochError{"Failed to initiate the font face store."}; }
}

Epoch::TextRenderer::FreetypeFontfaceLoader::~FreetypeFontfaceLoader()
{
	FT_Done_FreeType(m_handle);
}

FT_Face Epoch::TextRenderer::FreetypeFontfaceLoader::createFrom(std::byte const* src_buffer,
                                                                size_t src_buffer_size)
{
	FT_Face face;
	if(FT_New_Memory_Face(
	      m_handle, reinterpret_cast<FT_Byte const*>(src_buffer), src_buffer_size, 0, &face)
	   != FT_Err_Ok)
	{
		throw EpochError{FormatString{"Failed to load any font face from the current memory buffer."}};
	}

	return face;
}