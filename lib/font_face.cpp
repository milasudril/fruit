//@	{ "target": {"name":"font_face.o"}}

#include "./font_face.hpp"

#include <stdexcept>

namespace
{
	class FontfaceLoaderInitError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{ return "Failed to create a FreeType instance"; }
	};

	class FontfaceLoadError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{ return "Failed to load a fontface from current buffer"; }
	};
}

fruit::FontfaceLoader::FontfaceLoader()
{
	FT_Library handle{};
	if(auto res = FT_Init_FreeType(&handle); res != FT_Err_Ok)
	{ throw FontfaceLoaderInitError{}; }

	m_handle.reset(handle);
}

fruit::FontFace::FontFace(std::reference_wrapper<FontfaceLoader const> ft,
	                                      std::vector<std::byte>&& src_buffer):
	m_data{std::move(src_buffer)}
{
	FT_Face face{};
	if(FT_New_Memory_Face(ft.get().native_handle(),
		reinterpret_cast<FT_Byte const*>(std::data(m_data)), std::size(m_data),
		0, &face) != FT_Err_Ok)
	{
		throw FontfaceLoadError{};
	}
	m_handle.reset(face);
}