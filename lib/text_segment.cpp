//@	 {
//@	  "target":{"name":"text_segment.o"}
//@	 }

#include "./text_segment.hpp"

#include <stdexcept>

namespace
{
	class TextShapeResultError:public std::exception
	{
	public:
		char const* what() const noexcept override
		{
			return "Failed to copy text shaping result buffers";
		}
	};
}

fruit::TextShapeResult::TextShapeResult(uint32_t num_glyphs,
							hb_glyph_info_t const* info,
							hb_glyph_position_t const* geom,
							std::reference_wrapper<FreetypeFontFace const> font):
	m_glyph_count{num_glyphs},
	m_font{font}
{
	auto glyph_info = std::make_unique<GlyphInfo[]>(num_glyphs);
	auto glyph_geom = std::make_unique<GlyphGeometry[]>(num_glyphs);

	if(glyph_info == nullptr || glyph_geom == nullptr)
	{
		throw TextShapeResultError{};
	}

	std::transform(info, info + num_glyphs, glyph_info.get(), [](auto const item) {
		return GlyphInfo{item.codepoint, item.cluster};
	});

	std::transform(geom, geom + num_glyphs, glyph_geom.get(), [](auto const& item) {
		return GlyphGeometry{Vector{item.x_advance, item.y_advance, 0},
			Vector{item.x_offset, item.y_offset, 0}};
	});
}

void fruit::TextSegment::text_impl(std::basic_string_view<char8_t> buffer)
{
	auto const handle = native_handle();
	// https://lists.freedesktop.org/archives/harfbuzz/2016-July/005711.html
	auto const dir = direction();
	auto const s = script();
	// Do not want to convert to string and back
	auto lang = hb_buffer_get_language(handle);

	hb_buffer_reset(handle);
	auto const data = reinterpret_cast<char const*>(std::data(buffer));
	auto const size = std::size(buffer);
	hb_buffer_add_utf8(handle, data, size, 0, size);

	hb_buffer_set_language(handle, lang);
	direction(dir).script(s);
}

fruit::TextShapeResult fruit::TextSegment::shape_impl(TextShaper const& shaper) const
{
	auto const handle = m_handle.get();
	auto const shaper_ref = shaper.native_handle();

	hb_shape(shaper_ref, handle, nullptr, 0);
	unsigned int glyph_count{};
	auto const glyph_info = hb_buffer_get_glyph_infos(handle, &glyph_count);
	auto const glyph_pos = hb_buffer_get_glyph_positions(handle, &glyph_count);

	return TextShapeResult{glyph_count, glyph_info, glyph_pos, shaper.font()};
}