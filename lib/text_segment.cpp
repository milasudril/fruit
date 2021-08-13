//@	 {
//@	  "target":{"name":"text_segment.o"}
//@	 }

#include "./text_segment.hpp"
#include "./error_message.hpp"

#include <stdexcept>
#include <numeric>

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
							std::reference_wrapper<FontFace const> font,
							TextDirection direction,
							int char_height):
	m_glyph_count{num_glyphs},
	m_font{font},
	m_direction{direction},
	m_char_height{char_height}
{
	auto glyph_info = std::make_unique<GlyphInfo[]>(num_glyphs);
	auto glyph_geom = std::make_unique<GlyphGeometry[]>(num_glyphs);

	if(glyph_info == nullptr || glyph_geom == nullptr)
	{
		throw TextShapeResultError{};
	}

	std::transform(info, info + num_glyphs, glyph_info.get(), [](auto const item) {
		return GlyphInfo{GlyphIndex{item.codepoint}, item.cluster};
	});

	std::transform(geom, geom + num_glyphs, glyph_geom.get(), [](auto const& item) {
		return GlyphGeometry{Vector{item.x_advance, -item.y_advance, 0},
			Vector{item.x_offset, -item.y_offset, 0}};
	});

	m_glyph_info = std::move(glyph_info);
	m_glyph_geometry = std::move(glyph_geom);
}

namespace
{
	fruit::ViewportSize bounding_box_vertical(fruit::TextShapeResult const& shape_result, int size)
	{
		auto const geom = shape_result.glyph_geometry();
		auto const glyphs = shape_result.glyph_info();

		auto location = fruit::Origin<int>;
		auto width = 0;
		for(size_t k = 0; k < std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			width = std::max(width, src.width());
			location += geom[k].cursor_increment;
		}

		return fruit::ViewportSize{width, location.y()/64};
	}

	fruit::ViewportSize bounding_box_horizontal(fruit::TextShapeResult const& shape_result)
	{
		auto const geom = shape_result.glyph_geometry();
		auto const glyphs = shape_result.glyph_info();
		auto const size = shape_result.char_height();

		auto location = fruit::Origin<int>;
		auto height = 0;
		for(size_t k = 0; k < std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			auto const render_pos = (location + geom[k].render_offset - fruit::Origin<int>)/64 + glyph.render_offset;
			height = std::max(height, render_pos.y() + src.height());
			location += geom[k].cursor_increment;
		}

		return fruit::ViewportSize{location.x()/64, height};
	}

	fruit::Image<uint8_t> render_horizontal(fruit::TextShapeResult const& shape_result)
	{
		auto const size = shape_result.char_height();
		auto bb = bounding_box_horizontal(shape_result);
		FRUIT_ASSERT(bb.width > 0);
		FRUIT_ASSERT(bb.height > 0);

		fruit::Image<uint8_t> buffer{bb.width, bb.height};
		auto glyphs = shape_result.glyph_info();
		auto geom = shape_result.glyph_geometry();
		auto location = fruit::Origin<int>;
		for(size_t k = 0; k < std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			auto render_pos = (location + geom[k].render_offset - fruit::Origin<int>)/64 + glyph.render_offset;
			for(int k = 0; k < src.height(); ++k)
			{
				for(int l = 0; l < src.width(); ++l)
				{
					buffer(l + render_pos.x(), k + render_pos.y()) = src(l, k);
				}
			}
			location += geom[k].cursor_increment;
		}
		return buffer;
	}

	fruit::Image<uint8_t> render_vertical(fruit::TextShapeResult const& shape_result)
	{
		auto const size = shape_result.char_height();
		auto bb = bounding_box_vertical(shape_result, size);
		FRUIT_ASSERT(bb.width > 0);
		FRUIT_ASSERT(bb.height > 0);

		fruit::Image<uint8_t> buffer{bb.width, bb.height};
		auto glyphs = shape_result.glyph_info();
		auto geom = shape_result.glyph_geometry();
		auto location = fruit::Origin<int>;
		for(size_t k = 0; k < std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			auto render_pos = (location + geom[k].render_offset.y()*fruit::Y<int> - fruit::Origin<int>)/64
				+ glyph.render_offset
				+ (bb.width - src.width())*fruit::X<int>/2;

			for(int k = 0; k < src.height(); ++k)
			{
				for(int l = 0; l < src.width(); ++l)
				{
					buffer(l + render_pos.x(), k + render_pos.y()) = src(l, k);
				}
			}
			location += geom[k].cursor_increment;
		}
		return buffer;
	}
}

fruit::Image<uint8_t> fruit::render(TextShapeResult const& shape_result)
{
	return is_vertical(shape_result.direction())?
		render_vertical(shape_result):
		render_horizontal(shape_result);
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
	FT_Set_Pixel_Sizes(shaper.font().native_handle(), 0, shaper.char_height());
	hb_ft_font_changed(shaper_ref);
	hb_shape(shaper_ref, handle, nullptr, 0);
	unsigned int glyph_count{};
	auto const glyph_info = hb_buffer_get_glyph_infos(handle, &glyph_count);
	auto const glyph_pos = hb_buffer_get_glyph_positions(handle, &glyph_count);

	return TextShapeResult{glyph_count, glyph_info, glyph_pos, shaper.font(), direction(), shaper.char_height()};
}