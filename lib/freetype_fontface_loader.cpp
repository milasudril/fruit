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

fruit::FreetypeFontFace::FreetypeFontFace(std::reference_wrapper<FreetypeFontfaceLoader const> ft,
	                                      std::vector<std::byte>&& src_buffer):
	m_data{std::move(src_buffer)}
{
	FT_Face face{};
	if(FT_New_Memory_Face(ft.get().native_handle(),
		reinterpret_cast<FT_Byte const*>(std::data(m_data)), std::size(m_data),
		0, &face) != FT_Err_Ok)
	{
		throw FreetypeLoadError{};
	}
	m_handle.reset(face);
}