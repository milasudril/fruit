//@	{
//@	"target":
//@		{
//@		"name":"freetype_fontface_loader.o","pkgconfig_libs":["freetype2"]
//@		}
//@	}

#include "./freetype_fontface_loader.hpp"

#include <stdexcept>

namespace
{
	class FreetypeInitError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{ return "Failed to create a FreeType instance"; }
	};

	class FreetypeLoadError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{ return "Failed to load a fontface from current buffer"; }
	};
}

fruit::FreetypeFontfaceLoader::FreetypeFontfaceLoader()
{
	FT_Library handle{};
	if(auto res = FT_Init_FreeType(&handle); res != FT_Err_Ok)
	{ throw FreetypeInitError{}; }

	m_handle.reset(handle);
}

FT_Face fruit::FreetypeFontfaceLoader::createFrom(std::span<std::byte const> src_buffer)
{
	FT_Face face{};
	if(FT_New_Memory_Face(
	      m_handle.get(), reinterpret_cast<FT_Byte const*>(src_buffer.data()), src_buffer.size(), 0, &face)
	   != FT_Err_Ok)
	{
		throw FreetypeLoadError{};
	}

	return face;
}