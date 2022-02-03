//@	 {
//@	  "target":{"name":"text_segment.o"}
//@	 }

#include "./text_segment.hpp"
#include "./error_message.hpp"

#include <stdexcept>
#include <numeric>

namespace
{
	struct Deleter
	{
		void operator()(hb_buffer_t* hb)
		{
			if(hb != nullptr)
			{ hb_buffer_destroy(hb); }
		}
	};
}

fruit::TextShapeResult fruit::TextSegment::shape_impl(TextShaper const& shaper) const
{
	std::unique_ptr<hb_buffer_t, Deleter> ptr{hb_buffer_create()};
	auto const handle = ptr.get();
	if(!hb_buffer_allocation_successful(handle))
	{ FRUIT_JAM("Failed to allocate hb_buffer"); }

	auto const n = std::size(m_text);
	hb_buffer_add_utf8(handle, reinterpret_cast<char const*>(std::data(m_text)), n, 0, n);
	hb_buffer_set_language(handle, hb_language_from_string(m_properties.language.c_str(), -1));
	hb_buffer_set_direction(handle, static_cast<hb_direction_t>(m_properties.direction));
	hb_buffer_set_script(handle, static_cast<hb_script_t>(m_properties.script));
	FT_Set_Pixel_Sizes(shaper.font().native_handle(), 0, shaper.char_height());
	hb_ft_font_changed(shaper.native_handle());
	hb_shape(shaper.native_handle(), handle, nullptr, 0);
	unsigned int glyph_count{};
	auto const glyph_info = hb_buffer_get_glyph_infos(handle, &glyph_count);
	auto const glyph_pos = hb_buffer_get_glyph_positions(handle, &glyph_count);

	return TextShapeResult{glyph_count, glyph_info, glyph_pos, shaper.font(), direction(), shaper.char_height()};
}