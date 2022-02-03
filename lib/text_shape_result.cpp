//@	{"target":{"name":"text_shape_result.o"}}

#include "./text_shape_result.hpp"
#include "./viewport_size.hpp"
#include "./point.hpp"

class TextShapeResultError:public std::exception
{
public:
	char const* what() const noexcept override
	{
		return "Failed to copy text shaping result buffers";
	}
};

fruit::TextShapeResult::TextShapeResult(uint32_t num_glyphs,
							hb_glyph_info_t const* info,
							hb_glyph_position_t const* geom,
							std::reference_wrapper<FontFace const> font,
							TextDirection direction,
							int char_height):
	m_glyph_count{num_glyphs},
	m_font{&font.get()},
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
			width = std::max(width, static_cast<int>(src.width()));
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
			height = std::max(height, render_pos.y() + static_cast<int>(src.height()));
			location += geom[k].cursor_increment;
		}

		return fruit::ViewportSize{location.x()/64, height};
	}

	fruit::TextAlphaMask render_horizontal(fruit::TextShapeResult const& shape_result)
	{
		auto const size = shape_result.char_height();
		auto bb = bounding_box_horizontal(shape_result);
		FRUIT_ASSERT(bb.width > 0);
		FRUIT_ASSERT(bb.height > 0);

		fruit::TextAlphaMask buffer{static_cast<uint32_t>(bb.width), static_cast<uint32_t>(bb.height)};
		auto glyphs = shape_result.glyph_info();
		auto geom = shape_result.glyph_geometry();
		auto location = fruit::Origin<int>;
		for(size_t k = 0; k != std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			auto render_pos = (location + geom[k].render_offset - fruit::Origin<int>)/64 + glyph.render_offset;
			for(size_t k = 0; k != src.height(); ++k)
			{
				for(size_t l = 0; l != src.width(); ++l)
				{
					buffer(l + render_pos.x(), k + render_pos.y()) = src(l, k);
				}
			}
			location += geom[k].cursor_increment;
		}
		return buffer;
	}

	fruit::TextAlphaMask render_vertical(fruit::TextShapeResult const& shape_result)
	{
		auto const size = shape_result.char_height();
		auto bb = bounding_box_vertical(shape_result, size);
		FRUIT_ASSERT(bb.width > 0);
		FRUIT_ASSERT(bb.height > 0);

		fruit::TextAlphaMask buffer{static_cast<uint32_t>(bb.width), static_cast<uint32_t>(bb.height)};
		auto glyphs = shape_result.glyph_info();
		auto geom = shape_result.glyph_geometry();
		auto location = fruit::Origin<int>;
		for(size_t k = 0; k != std::size(glyphs); ++k)
		{
			auto const glyph = shape_result.font().render(glyphs[k].index, shape_result.direction(), size);
			auto const src = glyph.image;
			auto render_pos = (location + geom[k].render_offset.y()*fruit::Y<int> - fruit::Origin<int>)/64
				+ glyph.render_offset
				+ static_cast<int>(bb.width - src.width())*fruit::X<int>/2;

			for(size_t k = 0; k != src.height(); ++k)
			{
				for(size_t l = 0; l != src.width(); ++l)
				{
					buffer(l + render_pos.x(), k + render_pos.y()) = src(l, k);
				}
			}
			location += geom[k].cursor_increment;
		}
		return buffer;
	}
}

fruit::TextAlphaMask fruit::render(TextShapeResult const& shape_result)
{
	return is_vertical(shape_result.direction())?
		render_vertical(shape_result):
		render_horizontal(shape_result);
}