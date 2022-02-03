//@	{"dependencies_extra":[{"ref":"./text_shape_result.o", "rel":"implementation"}]}

#ifndef FRUIT_LIB_TEXT_SHAPE_RESULT_HPP
#define FRUIT_LIB_TEXT_SHAPE_RESULT_HPP

#include "./chartypes.hpp"
#include "./vector.hpp"
#include "./font_face.hpp"
#include "./error_message.hpp"

#include "pixel_store/image.hpp"

#include <hb.h>

namespace fruit
{
	struct GlyphInfo
	{
		GlyphIndex index;
		uint32_t start_offset;
	};

	struct GlyphGeometry
	{
		Vector<int> cursor_increment{0, 0, 0};
		Vector<int> render_offset{0, 0, 0};
	};

	class TextShapeResult
	{
	public:
		TextShapeResult():m_glyph_count{0}, m_font{nullptr}, m_direction{}, m_char_height{0}{}

		bool valid() const { return m_glyph_count != 0; }

		explicit TextShapeResult(uint32_t num_glyphs,
		                         hb_glyph_info_t const* info,
		                         hb_glyph_position_t const* geom,
		                         std::reference_wrapper<FontFace const> font,
		                         TextDirection direction,
		                         int char_height);

		std::span<GlyphInfo const> glyph_info() const
		{
			FRUIT_ASSERT(m_glyph_info.get() != nullptr);
			return std::span{m_glyph_info.get(), m_glyph_count};
		}

		std::span<GlyphGeometry const> glyph_geometry() const
		{
			FRUIT_ASSERT(m_glyph_geometry.get() != nullptr);
			return std::span{m_glyph_geometry.get(), m_glyph_count};
		}

		FontFace const& font() const
		{
			return *m_font;
		}

		int char_height() const
		{
			return m_char_height;
		}

		TextDirection direction() const
		{
			return m_direction;
		}

		size_t glyph_count() const { return m_glyph_count; }

	private:
		size_t m_glyph_count;
		std::unique_ptr<GlyphInfo[]> m_glyph_info;
		std::unique_ptr<GlyphGeometry[]> m_glyph_geometry;
		FontFace const* m_font;
		TextDirection m_direction;
		int m_char_height;
	};

	using TextAlphaMask = pixel_store::image<uint8_t>;

	TextAlphaMask render(TextShapeResult const& res);
}
#endif